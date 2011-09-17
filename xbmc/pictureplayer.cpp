#include "pictureplayer.h"
#include "xbmcconnection.h"

PicturePlayer::PicturePlayer(QObject *parent) :
    QObject(parent)
{
}

void PicturePlayer::playPause()
{
    XbmcConnection::sendCommand("PicturePlayer.PlayPause");
}

void PicturePlayer::moveUp()
{
    XbmcConnection::sendCommand("PicturePlayer.MoveUp");
}

void PicturePlayer::moveDown()
{
    XbmcConnection::sendCommand("PicturePlayer.MoveDown");
}

void PicturePlayer::moveLeft()
{
    XbmcConnection::sendCommand("PicturePlayer.MoveLeft");
}

void PicturePlayer::moveRight()
{
    XbmcConnection::sendCommand("PicturePlayer.MoveRight");
}

void PicturePlayer::rotate()
{
    XbmcConnection::sendCommand("PicturePlayer.Rotate");
}

void PicturePlayer::skipNext()
{
    XbmcConnection::sendCommand("PicturePlayer.SkipNext");
}

void PicturePlayer::skipPrevious()
{
    XbmcConnection::sendCommand("PicturePlayer.SkipPrevious");
}

void PicturePlayer::stop()
{
    XbmcConnection::sendCommand("PicturePlayer.Stop");
}

void PicturePlayer::zoomIn()
{
    XbmcConnection::sendCommand("PicturePlayer.ZoomIn");
}

void PicturePlayer::zoomOut()
{
    XbmcConnection::sendCommand("PicturePlayer.ZoomOut");
}

void PicturePlayer::zoom(int level)
{
    QVariantMap params;
    params.insert("value", level);
    XbmcConnection::sendCommand("PicturePlayer.Zoom", level);
}
