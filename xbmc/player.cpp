#include "player.h"
#include "playlist.h"

#include "xbmcconnection.h"

namespace Xbmc
{

Player::Player(PlayerType type, QObject *parent) :
    QObject(parent),
    m_type(type)
{

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

}
