#include "audioplaylistitem.h"

AudioPlaylistItem::AudioPlaylistItem(int albumId, int artistId, const QString &file, int genreId, const QString &playlist):
    m_albumId(albumId),
    m_artistId(artistId),
    m_genreId(genreId)
{
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
    if(m_albumId != -1){
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
