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

#include "playlistitem.h"

PlaylistItem::PlaylistItem(int albumId, int artistId, const QString &file, int genreId, const QString &playlist):
    m_albumId(albumId),
    m_artistId(artistId),
    m_file(file),
    m_genreId(genreId),
    m_playlistId(playlist)
{
}

void PlaylistItem::setAlbumId(int albumId)
{
    m_albumId = albumId;
}

void PlaylistItem::setArtistId(int artistId)
{
    m_artistId = artistId;
}

void PlaylistItem::setFile(const QString &file)
{
    m_file = file;
}

void PlaylistItem::setGenreId(int genreId)
{
    m_genreId = genreId;
}

void PlaylistItem::setPlayList(const QString &playlist)
{
    m_playlistId = playlist;
}

int PlaylistItem::albumId() const
{
    return m_albumId;
}

int PlaylistItem::artistId() const
{
    return m_artistId;
}

QString PlaylistItem::file() const
{
    return m_file;
}

int PlaylistItem::genreId() const
{
    return m_genreId;
}

QString PlaylistItem::playlistId() const
{
    return m_playlistId;
}
QVariantMap PlaylistItem::toMap() const
{
    QVariantMap map;
    if(m_albumId != -1){
        map.insert("albumid", m_albumId);
    }
    if(m_artistId != -1) {
        map.insert("artistid", m_artistId);
    }
    if(!m_file.isEmpty()) {
        map.insert("file", m_file);
    }
    if(m_genreId != -1) {
        map.insert("genreid", m_genreId);
    }
    if(!m_playlistId.isEmpty()) {
        QVariantMap playlist;
        playlist.insert("id", m_playlistId);
        map.insert("playlist", playlist);
    }
    return map;
}
