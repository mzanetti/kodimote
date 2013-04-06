/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "eventclient.h"
#include "xbmchostmodel.h"

#include <sys/socket.h>

#include "xdebug.h"
#include <QPixmap>

EventClient::EventClient(QObject *parent) :
    QObject(parent),
    m_socket(-1),
    m_cleared(0)
{
    m_releaseTimer.setSingleShot(false);
    m_releaseTimer.setInterval(200);
    connect(&m_releaseTimer, SIGNAL(timeout()), SLOT(releaseButton()));
}

void EventClient::setApplicationThumbnail(const QString &thumbnail)
{
    m_thumbnail = thumbnail;
}

void EventClient::connectToHost(XbmcHost *host)
{
    if (m_socket >= 0)  {
        disconnectFromHost();
    }

    m_xbmcHost = CAddress(host->address().toLatin1().data());

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket < 0) {
        qDebug() << "cannot create socket";
    }
    m_xbmcHost.Bind(m_socket);

    if (!m_thumbnail.isEmpty()) {
        CPacketHELO HeloPackage("N9 Xbmcremote", ICON_PNG, m_thumbnail.toLatin1().data());
        HeloPackage.Send(m_socket, m_xbmcHost);
    } else {
        CPacketHELO HeloPackage("N9 Xbmcremote", ICON_NONE);
        HeloPackage.Send(m_socket, m_xbmcHost);
    }

    qDebug() << "connected to xbmc" << m_socket;
}

void EventClient::disconnectFromHost()
{
    if (m_socket >= 0) {
        CPacketBYE bye; // CPacketPing if you want to ping
        bye.Send(m_socket, m_xbmcHost);

        int on = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
            qDebug() << "cannot reuse socket";
        }

        m_socket = -1;

    }
}

void EventClient::sendKeypress(const QString &buttonName)
{
    CPacketBUTTON btnDown(buttonName.toLatin1().data(), "KB", BTN_USE_NAME);
    btnDown.Send(m_socket, m_xbmcHost);

    m_cleared = 0;
    m_releaseTimer.start();
}

void EventClient::sendIrPress(const QString &buttonName)
{
    CPacketBUTTON btnDown(buttonName.toLatin1().data(), "LI:xbmcremote", BTN_USE_NAME);
    btnDown.Send(m_socket, m_xbmcHost);

    m_cleared = 0;
    m_releaseTimer.start();
}

void EventClient::releaseButton()
{
    CPacketBUTTON btnUp;
    btnUp.Send(m_socket, m_xbmcHost);

    // As eventserver is UDP based, it happens that some presses get lost.
    // Try to avoid it by releasing buttons twice
    if (++m_cleared >= 2) {
        m_releaseTimer.stop();
    }
}

