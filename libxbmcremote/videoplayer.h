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

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "player.h"
#include "videoplaylistitem.h"

class VideoPlaylist;

class VideoPlayer : public Player
{
    Q_OBJECT
public:
    VideoPlayer(QObject *parent = 0);

    virtual Playlist *playlist() const;

    void open(const VideoPlaylistItem &item);

protected:
    virtual int playerId() const;

private:
    VideoPlaylist *m_playlist;

};

#endif // VIDEOPLAYER_H
