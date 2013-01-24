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

#include "audioplaylistitem.h"

AudioPlaylistItem::AudioPlaylistItem(int albumId, int artistId, int genreId):
    m_songId(-1),
    m_albumId(albumId),
    m_artistId(artistId),
    m_genreId(genreId)
{
}

void AudioPlaylistItem::setSongId(int songId)
{
    m_songId = songId;
}

void AudioPlaylistItem::setAlbumId(int albumId)
{
    m_albumId = albumId;
}

void AudioPlaylistItem::setArtistId(int artistId)
{
    m_artistId = artistId;
}

void AudioPlaylistItem::setGenreId(int genreId)
{
    m_genreId = genreId;
}

int AudioPlaylistItem::songId() const
{
    return m_songId;
}

int AudioPlaylistItem::albumId() const
{
    return m_albumId;
}

int AudioPlaylistItem::artistId() const
{
    return m_artistId;
}

int AudioPlaylistItem::genreId() const
{
    return m_genreId;
}

QVariantMap AudioPlaylistItem::toMap() const
{
    QVariantMap map = PlaylistItem::toMap();
    if(m_songId != -1) {
        map.insert("songid", m_songId);
    }

    if(m_albumId != -1) {
        map.insert("albumid", m_albumId);
    }

    if(m_artistId != -1) {
        map.insert("artistid", m_artistId);
    }

    if(m_genreId != -1) {
        map.insert("genreid", m_genreId);
    }
    return map;
}

void AudioPlaylistItem::setArtist(const QString &artist)
{
    m_artist = artist;
    emit artistChanged();
}

QString AudioPlaylistItem::artist() const
{
    return m_artist;
}

void AudioPlaylistItem::setAlbum(const QString &album)
{
    m_album = album;
    emit albumChanged();
}

QString AudioPlaylistItem::album() const
{
    return m_album;
}
