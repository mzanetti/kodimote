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

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QString>
#include <QVariantMap>

class PlaylistItem
{
public:
    PlaylistItem(int albumId = -1, int artistId = -1, const QString &file = QString(), int genreId = -1, const QString &playlist = QString());

    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setFile(const QString &file);
    void setGenreId(int genreId);
    void setPlayList(const QString &playlist);

    int albumId() const;
    int artistId() const;
    QString file() const;
    int genreId() const;
    QString playlistId() const;

    QVariantMap toMap() const;

private:
    int m_albumId;
    int m_artistId;
    QString m_file;
    int m_genreId;
    QString m_playlistId;
};

#endif // PLAYLISTITEM_H
