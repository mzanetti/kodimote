/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
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
public:
    VideoPlaylist(Player *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

//    void clear();
    PlaylistItem* at(int index) const;
    int rowCount(const QModelIndex &parent) const;

    QString title() const;

public slots:
    void refresh();

protected:
    QString namespaceString() const;
    void queryItemData(int index);

private slots:
    void responseReveiced(int id, const QVariantMap &response);

private:
    QList<VideoPlaylistItem*> m_itemList;
};

#endif // VIDEOPLAYLIST_H
