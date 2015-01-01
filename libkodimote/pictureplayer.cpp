/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "pictureplayer.h"
#include "kodiconnection.h"
#include "kodi.h"
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
    KodiConnection::sendCommand("Player.PlayPause", params);
}

void PicturePlayer::moveUp()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "up");
    KodiConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveDown()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "down");
    KodiConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveLeft()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "left");
    KodiConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::moveRight()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("direction", "right");
    KodiConnection::sendCommand("Player.Move", params);
}

void PicturePlayer::rotate()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    KodiConnection::sendCommand("Player.Rotate", params);
}

void PicturePlayer::zoomIn()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("zoom", "in");
    KodiConnection::sendCommand("Player.Zoom", params);
}

void PicturePlayer::zoomOut()
{
    QVariantMap params;
    params.insert("playerid", playerId());
    params.insert("zoom", "out");
    KodiConnection::sendCommand("Player.Zoom", params);
}

void PicturePlayer::zoom(int level)
{
    QVariantMap params;
    params.insert("value", level);
    params.insert("playerid", playerId());
    KodiConnection::sendCommand("Player.Zoom", params);
}
