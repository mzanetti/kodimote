/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "xbmc.h"
#include "xbmchostmodel.h"
#include "xbmcconnection.h"
#include "xdebug.h"
#include "xbmcdiscovery.h"

#include "audiolibrary.h"
#include "artists.h"

#include "videolibrary.h"

#include "files.h"
#include "shares.h"

#include "playlist.h"
#include "audioplaylist.h"

#include "audioplayer.h"
#include "videoplayer.h"
#include "pictureplayer.h"

#include "audioplaylistitem.h"
#include "videoplaylistitem.h"

#include "keys.h"

#include "settings.h"

#include <QSettings>
#include <QtDeclarative>

Xbmc *Xbmc::s_instance = 0;

Xbmc *Xbmc::instance()
{
    if(!s_instance) {
        s_instance = new Xbmc();
    }
    return s_instance;
}

Xbmc::Xbmc(QObject *parent) :
    QObject(parent)
{
    qmlRegisterType<AudioLibrary>();
    qmlRegisterType<VideoLibrary>();
    qmlRegisterType<Artists>();
    qmlRegisterType<XbmcModel>();
    qmlRegisterType<LibraryItem>();
    qmlRegisterType<XbmcModelItem>();
    qmlRegisterType<XbmcLibrary>();
    qmlRegisterType<Player>();
    qmlRegisterType<AudioPlayer>();
    qmlRegisterType<VideoPlayer>();
    qmlRegisterType<PicturePlayer>();
    qmlRegisterType<Playlist>();
    qmlRegisterType<PlaylistItem>();
    qmlRegisterType<AudioPlaylistItem>();
    qmlRegisterType<VideoPlaylistItem>();
    qmlRegisterType<Files>();
    qmlRegisterType<Shares>();
    qmlRegisterType<Keys>();

    qmlRegisterType<AudioPlayer>("Xbmc", 1, 0, "Player");

    qmlRegisterType<XbmcHostModel>();
    qmlRegisterType<XbmcDiscovery>("Xbmc", 1, 0, "XbmcDiscovery");

    QSettings settings("xbmcremote");

    m_hosts = new XbmcHostModel(this);

    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), SLOT(connectionChanged()));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(authenticationRequired(QString,QString)), SIGNAL(authenticationRequired(QString, QString)));

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_picturePlayer = new PicturePlayer(this);
    m_activePlayer = m_audioPlayer;
    m_state = "undefined";
    m_picturePlayerActive = false;

    m_keys = new Keys(this);

    m_volumeAnimation.setTargetObject(this);
    m_volumeAnimation.setPropertyName("volume");
    m_volumeAnimation.setDuration(500);
}

Xbmc::~Xbmc()
{
}

void Xbmc::init()
{
    queryActivePlayers();

    QVariantMap params;
    QVariantList list;
    list.append("volume");
    params.insert("properties", list);
    int id = XbmcConnection::sendCommand("Application.GetProperties", params);
    m_requestMap.insert(id, RequestVolume);

}

bool Xbmc::connected()
{
    return XbmcConnection::connected();
}

XbmcHost *Xbmc::connectedHost()
{
    return XbmcConnection::connectedHost();
}

void Xbmc::setAuthCredentials(const QString &username, const QString &password)
{
    XbmcConnection::setAuthCredentials(username, password);
}

QString Xbmc::connectionError()
{
    return XbmcConnection::connectionError();
}

AudioLibrary *Xbmc::audioLibrary()
{
    return new AudioLibrary();
}

VideoLibrary *Xbmc::videoLibrary()
{
    return new VideoLibrary();
}

Shares *Xbmc::shares(const QString &mediatype)
{
    return new Shares(mediatype);
}

AudioPlayer *Xbmc::audioPlayer()
{
    return m_audioPlayer;
}

VideoPlayer *Xbmc::videoPlayer()
{
    return m_videoPlayer;
}

PicturePlayer *Xbmc::picturePlayer()
{
    return m_picturePlayer;
}

XbmcHostModel *Xbmc::hostModel()
{
    return m_hosts;
}

Player *Xbmc::activePlayer()
{
//    qDebug() << "returning active player" << m_activePlayer;
    return m_activePlayer;
}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    Q_UNUSED(map)
}

void Xbmc::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    Player *activePlayer = 0;

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestActivePlayer: {
        // {"id":8,"jsonrpc":"2.0","result":[{"playerid":0,"type":"audio"}]}
        xDebug(XDAREA_PLAYER) << "active player response:" << rsp;
        QVariantList activePlayers = rsp.toList();
        bool picturesActive = false;
        foreach(const QVariant &activePlayerMap, activePlayers) {
            if(activePlayerMap.toMap().value("type").toString() == "audio") {
                activePlayer = m_audioPlayer;
                m_state = "audio";
            } else if(activePlayerMap.toMap().value("type").toString() == "video") {
                activePlayer = m_videoPlayer;
                m_state = "video";
            }
            if(activePlayerMap.toMap().value("type").toString() == "picture"){
                picturesActive = true;
            }
        }

        if(activePlayer == 0) {
            m_state = "";
        } else if(m_activePlayer != activePlayer) {
            m_activePlayer = activePlayer;
            xDebug(XDAREA_PLAYER) << "active player changed!";
            emit stateChanged();
            emit activePlayerChanged();
        }
        if(m_activePlayer) {
            m_activePlayer->refresh();
        }

        if(m_picturePlayerActive != picturesActive) {
            m_picturePlayerActive = picturesActive;
            emit picturePlayerActiveChanged();
        }

        }
        break;
    case RequestVolume:
        m_volume = rsp.toMap().value("volume").toInt();
//        qDebug() << "Volume received" << m_volume;
        emit volumeChanged(m_volume);
        break;

    }

}

QString Xbmc::vfsPath()
{
    if(XbmcConnection::connectedHost()) {
        //xDebug(XDAREA_CONNECTION) << "returning vfs:" << "http://" + m_hostName + ':' + QString::number(m_port) + "/vfs/";
        return "http://" + XbmcConnection::connectedHost()->address() + ':' + QString::number(XbmcConnection::connectedHost()->port()) + "/vfs/";
    }
    return QString();
}

QString Xbmc::state()
{
    return m_state;
}

Keys *Xbmc::keys()
{
    return m_keys;
}

void Xbmc::connectionChanged()
{
    if(connected()) {
        init();
    }
    emit connectedChanged(connected());
}

void Xbmc::setVolume(int volume)
{
    if(volume < 0) {
        volume = 0;
    }
    if(volume > 100) {
        volume = 100;
    }

    Settings settings;
    if(!settings.volumeUpCommand().isEmpty()) {
        QProcess p;
        p.execute(settings.volumeUpCommand(), QStringList() << QString::number(volume));
    } else {
        if(volume != m_volume) {
            QVariantMap map;
            if(XbmcConnection::xbmcVersion() < QDate(2011, 10, 8)) {
                map.insert("value", volume);
            } else {
                map.insert("volume", volume);
            }
            XbmcConnection::sendCommand("Application.SetVolume", map);
            m_volume = volume;
            emit volumeChanged(m_volume);
        }
    }
}

int Xbmc::volume()
{
    return m_volume;
}

void Xbmc::quit()
{
    XbmcConnection::sendCommand("Application.Quit");
}

void Xbmc::dimVolumeTo(int newVolume)
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Forward);
    m_volumeAnimation.setStartValue(m_volume);
    m_volumeAnimation.setEndValue(newVolume);
    m_volumeAnimation.start();
}

void Xbmc::restoreVolume()
{
    m_volumeAnimation.setDirection(QAbstractAnimation::Backward);
    m_volumeAnimation.start();
}

bool Xbmc::picturePlayerActive()
{
    return m_picturePlayerActive;
}

void Xbmc::queryActivePlayers()
{
    int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
    m_requestMap.insert(id, RequestActivePlayer);
}
