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

#ifndef VIDEOPLAYLIST_H
#define VIDEOPLAYLIST_H

#include "playlist.h"
#include "videoplaylistitem.h"

class VideoPlaylist : public Playlist
{
    Q_OBJECT
public:
    VideoPlaylist(Player *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

//    void clear();
    PlaylistItem* at(int index) const;
    int rowCount(const QModelIndex &parent) const;

    QString title() const;
    int playlistId() const;

public slots:
    void refresh();

protected:
    void queryItemData(int index);

private slots:
    void itemsReceived(const QVariantMap &rsp);
    void currentDataReceived(const QVariantMap &rsp);

private:
    QList<VideoPlaylistItem*> m_itemList;
};

#endif // VIDEOPLAYLIST_H
