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

#ifndef AUDIOPLAYLISTITEM_H
#define AUDIOPLAYLISTITEM_H

#include "playlistitem.h"

class AudioPlaylistItem : public PlaylistItem
{
    Q_OBJECT
    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)

public:
    AudioPlaylistItem(int albumId = -1, int artistId = -1, int genreId = -1);

    // Those will be filled in on toMap()
    void setSongId(int songId);
    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setGenreId(int genreId);

    int songId() const;
    int albumId() const;
    int artistId() const;
    int genreId() const;

    virtual QVariantMap toMap() const;

    void setArtist(const QString &artist);
    void setAlbum(const QString &album);

    QString artist() const;
    QString album() const;

signals:
    void albumChanged();
    void artistChanged();

private:
    int m_songId;
    int m_albumId;
    int m_artistId;
    int m_genreId;

    QString m_artist;
    QString m_album;
};

#endif // AUDIOPLAYLISTITEM_H
