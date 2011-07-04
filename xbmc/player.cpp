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

#include <QTime>

Player::Player(PlayerType type, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_state("stopped"),
    m_speed(1),
    m_percentage(0)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    staticMetaObject.invokeMethod(this, "getState", Qt::QueuedConnection);

    m_percentageTimer.setInterval(1000);
    connect(&m_percentageTimer, SIGNAL(timeout()), SLOT(setPercentage()));
}

void Player::getState()
{
    int id = XbmcConnection::sendCommand(namespaceString() + ".State");
    m_requestMap.insert(id, RequestState);
}

Player::PlayerType Player::type() const
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

QString Player::state() const
{
    return m_state;
}

void Player::receivedAnnouncement(const QVariantMap &map)
{
    qDebug() << "got announcement:" << map.value("method");
    if(map.value("method").toString() == "Player.OnStop") {
        m_state = "stopped";
        m_percentageTimer.stop();
        emit stateChanged();
        m_speed = 1;
        emit speedChanged();
    } else if(map.value("method").toString() == "Player.OnPause") {
        m_state = "paused";
        m_percentageTimer.stop();
        emit stateChanged();
        m_speed = 1;
        emit speedChanged();
    } else if(map.value("method").toString() == "Player.OnPlay") {
        qDebug() << map.value("params").toMap().value("data");
        m_state = "playing";
        m_speed = map.value("params").toMap().value("data").toMap().value("speed").toInt();
        emit stateChanged();
        int id = XbmcConnection::sendCommand(namespaceString() + ".GetPercentage");
        m_requestMap.insert(id, RequestPercentage);
        emit speedChanged();
    }
}

void Player::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }
    if(response.contains("error")) {
        qDebug() << "Error:" << response.value("error");
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestState:
//        qDebug() << "****** got state" << rsp;
        if(rsp.toMap().value("paused").toBool()) {
            m_state = "paused";
            m_percentageTimer.stop();
        } else if(rsp.toMap().value("playing").toBool()) {
            m_state = "playing";
            int id = XbmcConnection::sendCommand(namespaceString() + ".GetPercentage");
            m_requestMap.insert(id, RequestPercentage);
        }
        emit stateChanged();
        break;
    case RequestPercentage:
        qDebug() << "*** Got percentage response" << response;
        m_percentage = rsp.toDouble();
        emit percentageChanged();
        m_percentageTimer.start();
    }
}

int Player::speed() const
{
    return m_speed;
}

double Player::percentage() const
{
    return m_percentage;
}

QString Player::time() const
{
    PlaylistItem *item = playlist()->currentItem();
    if(item) {
        QTime time;
        int currentTime = time.secsTo(playlist()->currentItem()->duration());
        currentTime = currentTime * m_percentage / 100;
        time = QTime();
        time = time.addSecs(currentTime);
        if(item->duration().hour() > 0) {
            return time.toString("hh:mm:ss");
        } else {
            return time.toString("mm:ss");
        }
    }
    return "00:00";
}

void Player::setPercentage()
{
    QTime time;
    int duration = time.secsTo(playlist()->currentItem()->duration());
    if(duration > 0) {
        m_percentage += 100.0 / duration * m_speed;
    } else {
        m_percentage = 100;
    }
    emit percentageChanged();
    emit timeChanged();
}
