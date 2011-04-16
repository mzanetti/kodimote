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
