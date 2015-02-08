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

#ifndef PICTUREPLAYLIST_H
#define PICTUREPLAYLIST_H

#include "playlist.h"

class PicturePlaylist : public Playlist
{
public:
    PicturePlaylist();

    int rowCount(const QModelIndex &) const { return 0; }
    QVariant data(const QModelIndex &, int) const { return "bla"; }
    QString title() const {return "Pictures"; }
    PlaylistItem *at(int) const { return 0; }
    int playlistId() const { return 2; }
    void refresh() {}
    void queryItemData(int) {}

    MediaFormat mediaFormat() const { return MediaFormatPictures; }

private slots:
    void receivedAnnouncement(const QVariantMap &map);

};

#endif // PICTUREPLAYLIST_H
