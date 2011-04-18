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

#include "player.h"
#include "playlist.h"

#include "xbmcconnection.h"

namespace Xbmc
{

Player::Player(PlayerType type, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_state("stopped"),
    m_speed(1)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    staticMetaObject.invokeMethod(this, "getState", Qt::QueuedConnection);
}

void Player::getState()
{
    int id = XbmcConnection::sendCommand(namespaceString() + ".State");
    m_requestMap.insert(id, RequestState);
}

Player::PlayerType Player::type()
{
    return m_type;
}


void Player::playPause()
{
    XbmcConnection::sendCommand(namespaceString() + ".PlayPause");
}

void Player::stop()
{
    XbmcConnection::sendCommand(namespaceString() + ".Stop");
}

void Player::skipPrevious()
{
    //    XbmcConnection::sendCommand(namespaceString() + ".SkipPrevious");
    playlist()->skipPrevious();
}

void Player::skipNext()
{
    //    XbmcConnection::sendCommand(namespaceString() + ".SkipNext");
    playlist()->skipNext();
}

void Player::seekBackward()
{
    XbmcConnection::sendCommand(namespaceString() + ".Rewind");
}

void Player::seekForward()
{
    XbmcConnection::sendCommand(namespaceString() + ".Forward");
}

QString Player::state()
{
    return m_state;
}

void Player::receivedAnnouncement(const QVariantMap &map)
{
    if(map.value("message").toString() == "PlaybackEnded" ||
            map.value("message").toString() == "PlaybackStopped") {
        m_state = "stopped";
        emit stateChanged();
    } else if(map.value("message").toString() == "PlaybackPaused") {
        m_state = "paused";
        emit stateChanged();
    } else if(map.value("message").toString() == "PlaybackStarted" ||
              map.value("message").toString() == "PlaybackResumed") {
        m_state = "playing";
        emit stateChanged();
    } else if(map.value("message").toString() == "PlaybackSpeedChanged") {
        m_speed = map.value("data").toMap().value("speed").toInt();
        emit speedChanged();
    }
}

void Player::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestState:
//        qDebug() << "****** got state" << rsp;
        if(rsp.toMap().value("paused").toBool()) {
            m_state = "paused";
        } else if(rsp.toMap().value("playing").toBool()) {
            m_state = "playing";
        }
        emit stateChanged();
    }
}

int Player::speed()
{
    return m_speed;
}

}
