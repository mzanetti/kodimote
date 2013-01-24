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

#ifndef PICTUREPLAYER_H
#define PICTUREPLAYER_H

#include "player.h"
#include <QObject>

class PicturePlayer: public Player
{
    Q_OBJECT
public:
    explicit PicturePlayer(QObject *parent = 0);

    int playerId() const;
    Playlist *playlist() const;

public slots:
    void playPause();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void rotate();
    void zoomIn();
    void zoomOut();

    /**
      * Min: 1, Max: 10
      */
    void zoom(int level);

private:
    Playlist *m_playlist;
};

#endif // PICTUREPLAYER_H
