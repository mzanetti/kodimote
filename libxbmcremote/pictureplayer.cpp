#include "pictureplayer.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "pictureplaylist.h"

PicturePlayer::PicturePlayer(QObject *parent) :
    Player(PlayerTypePictures, parent)
{
    m_playlist = new PicturePlaylist();
}

int PicturePlayer::playerId() const
{
    return 2;
}

Playlist *PicturePlayer::playlist() const
{
    return m_playlist;
}

void PicturePlayer::playPause()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    XbmcConnection::sendCommand("Player.PlayPause", params);
}

void PicturePlayer::moveUp()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "up");
    XbmcConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveDown()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "down");
    XbmcConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveLeft()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "left");
    XbmcConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveRight()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "right");
    XbmcConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::rotate()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    XbmcConnection::sendCommand("Player.Rotate", params);
}

//void PicturePlayer::skipNext()
//{
//    QVariantMap params;
//    params.insert("playerid", playerId());
//    XbmcConnection::sendCommand("Player.GoNext", params);
//}

//void PicturePlayer::skipPrevious()
//{
//    QVariantMap params;
//    params.insert("playerid", playerId());
//    XbmcConnection::sendCommand("Player.GoPrevious", params);
//}

//void PicturePlayer::stop()
//{
//    QVariantMap params;
//    params.insert("playerid", playerId());
//    XbmcConnection::sendCommand("Player.Stop", params);
//    QTimer::singleShot(100, Xbmc::instance(), SLOT(queryActivePlayers()));
//    m_state == "stopped";
//}

void PicturePlayer::zoomIn()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("zoom", "in");
    XbmcConnection::sendCommand("Player.Zoom", params);
}

void PicturePlayer::zoomOut()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("zoom", "out");
    XbmcConnection::sendCommand("Player.Zoom", params);
}

void PicturePlayer::zoom(int level)
{
    QVariantMap params;
    params.insert("value", level);
    params.insert("playerid", playerId());
    XbmcConnection::sendCommand("Player.Zoom", params);
}
