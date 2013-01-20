#include "eventclient.h"
#include "xbmchostmodel.h"

#include <sys/socket.h>

#include "xdebug.h"
#include <QPixmap>

EventClient::EventClient(QObject *parent) :
    QObject(parent),
    m_socket(-1)
{
    m_releaseTimer.setSingleShot(true);
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

    m_releaseTimer.start();
}

void EventClient::sendIrPress(const QString &buttonName)
{
    CPacketBUTTON btnDown(buttonName.toLatin1().data(), "LI:xbmcremote", BTN_USE_NAME);
    btnDown.Send(m_socket, m_xbmcHost);

    m_releaseTimer.start();
}

void EventClient::releaseButton()
{
    CPacketBUTTON btnUp;
    btnUp.Send(m_socket, m_xbmcHost);
}

