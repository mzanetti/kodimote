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
#include "xbmcconnection.h"

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

    qmlRegisterType<AudioPlaylist>("Xbmc", 1, 0, "Playlist");

    QSettings settings("xbmcremote");
    m_hostname = settings.value("Host").toString();
    m_port = settings.value("Port", 8080).toInt();
    m_username = settings.value("Username").toString();
    m_password = settings.value("Password").toString();

    XbmcConnection::connect(m_hostname, m_port, m_username, m_password);
//    XbmcConnection::connect("10.10.10.10", 8080);

    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), SLOT(connectionChanged()));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
//    m_picturePlayer = new PicturePlayer(this);
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

//PicturePlayer *Xbmc::picturePlayer()
//{
//    return m_picturePlayer;
//}

QString Xbmc::hostname()
{
    return m_hostname;
}

void Xbmc::setHostname(const QString &hostname)
{
    QSettings settings("xbmcremote");
    settings.setValue("Host", hostname);
    m_hostname = hostname;
    emit hostnameChanged();
}

int Xbmc::port(){
    qDebug() << "port" << m_port;
    return m_port;
}

void Xbmc::setPort(int port)
{
    QSettings settings("xbmcremote");
    settings.setValue("Port", port);
    m_port = port;
    emit portChanged();
}

QString Xbmc::username()
{
    return m_username;
}

void Xbmc::setUsername(const QString &username)
{
    QSettings settings("xbmcremote");
    settings.setValue("Username", username);
    m_username = username;
    emit usernameChanged();
}

QString Xbmc::password()
{
    return m_password;
}

void Xbmc::setPassword(const QString &password)
{
    QSettings settings("xbmcremote");
    settings.setValue("Password", password);
    m_password = password;
    emit passwordChanged();
}

void Xbmc::connectToHost()
{
    XbmcConnection::connect(m_hostname, m_port, m_username, m_password);
}

Player *Xbmc::activePlayer()
{
    return m_activePlayer;
}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();
    qDebug() << "incoming announcement" << map << "method:" << method;
    if(method == "Player.OnPlay" || method == "Player.OnStop") {
        int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
        m_requestMap.insert(id, RequestActivePlayer);
    }
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
        qDebug() << "active player response:" << rsp;
        QVariantMap activePlayerMap = rsp.toMap();
        if(activePlayerMap.value("audio").toBool() == true) {
            activePlayer = m_audioPlayer;
            m_state = "audio";
        } else if(activePlayerMap.value("video").toBool() == true) {
            activePlayer = m_videoPlayer;
            m_state = "video";
        } else {
            activePlayer = 0;
            m_state = "";
        }
        if(activePlayerMap.value("picture").toBool() == true) {
            if(!m_picturePlayerActive) {
                m_picturePlayerActive = true;
                emit picturePlayerActiveChanged();
            }
        } else {
            if(m_picturePlayerActive) {
                m_picturePlayerActive = false;
                emit picturePlayerActiveChanged();
            }
        }
        if(m_activePlayer != activePlayer) {
            m_activePlayer = activePlayer;
            qDebug() << "active player changed!";
            emit stateChanged();
            emit activePlayerChanged();
        }
        if(m_activePlayer) {
            m_activePlayer->playlist()->refresh();
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
    return XbmcConnection::vfsPath();
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
    qDebug() << "Connection changed to " << connected();
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
            map.insert("value", volume);
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
    XbmcConnection::sendCommand("XBMC.Quit");
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

void Xbmc::startSlideShow(const QString &directory)
{
    QVariantMap params;
    params.insert("directory", directory);

    XbmcConnection::sendCommand("XBMC.StartSlideShow", params);
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
