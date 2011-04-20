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

#include "playlist.h"
#include "audiolibrary.h"
#include "audioplayer.h"
#include "videoplayer.h"
#include "files.h"

#include <QDebug>

namespace Xbmc
{

Xbmc::Xbmc(QObject *parent):
    QObject(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(parseAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), SIGNAL(connectedChanged()));

    int id = XbmcConnection::sendCommand("Player.GetActivePlayers");
    m_requestMap.insert(id, RequestActivePlayer);

    id = XbmcConnection::sendCommand("XBMC.GetVolume");
    m_requestMap.insert(id, RequestVolume);

    m_audioPlayer = new AudioPlayer(this);
    m_videoPlayer = new VideoPlayer(this);
    m_activePlayer = 0;

    m_audioLibrary = new AudioLibrary(m_audioPlayer, this);

    m_files = new Files(m_audioPlayer, this);
}

bool Xbmc::connected() {
    return XbmcConnection::connected();
}

QString Xbmc::state()
{
    if(m_activePlayer == m_audioPlayer) {
        return "audio";
    }
    if(m_activePlayer == m_videoPlayer) {
        return "video";
    }
    return "undefined";
}

Player *Xbmc::audioPlayer()
{
    return m_audioPlayer;
}

AudioLibrary *Xbmc::audioLibrary()
{
    return m_audioLibrary;
}

Files *Xbmc::files()
{
    return m_files;
}

Player *Xbmc::videoPlayer()
{
    return m_videoPlayer;
}

Player *Xbmc::activePlayer()
{
    return m_activePlayer;
}

int Xbmc::volume()
{
    return m_volume;
}

void Xbmc::setVolume(int volume)
{
    if(volume != m_volume) {
        QVariantMap map;
        map.insert("value", volume);
        int id = XbmcConnection::sendCommand("XBMC.SetVolume", map);
        m_volume = volume;
//        m_requestMap.insert(id, RequestVolume);
    }
}

void Xbmc::toggleMute()
{

}

void Xbmc::parseAnnouncement(const QVariantMap &map)
{
    if(map.value("method").toString() == "Player.PlaybackStarted") {
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
//        qDebug() << "active player response:" << rsp;
        QVariantMap activePlayerMap = rsp.toMap();
        if(activePlayerMap.value("audio").toBool() == true) {
            activePlayer = m_audioPlayer;
        } else if(activePlayerMap.value("video").toBool() == true) {
            activePlayer = m_videoPlayer;
        } else if(activePlayerMap.value("pictures").toBool() == true) {
//            activePlayer = m_picturePlayer;
        }
        if(m_activePlayer != activePlayer) {
            m_activePlayer = activePlayer;
            qDebug() << "active player changed!";
            emit stateChanged();
            emit activePlayerChanged(m_activePlayer);
        }
        if(m_activePlayer) {
            m_activePlayer->playlist()->refresh();
        }
        }
        break;
    case RequestVolume:
        m_volume = rsp.toInt();
//        qDebug() << "Volume received" << m_volume;
        emit volumeChanged(m_volume);
        break;

    }

}
QString Xbmc::vfsPath()
{
    return XbmcConnection::vfsPath();
}

}
