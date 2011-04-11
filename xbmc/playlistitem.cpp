#include "playlistitem.h"

namespace Xbmc
{

PlaylistItem::PlaylistItem(int albumId, int artistId, const QString &file, int genreId, int playlistId):
    m_albumId(albumId),
    m_artistId(artistId),
    m_file(file),
    m_genreId(genreId),
    m_playlistId(playlistId)
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

void PlaylistItem::setPlayList(int playlistId)
{
    m_playlistId = playlistId;
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

int PlaylistItem::playlistId() const
{
    return m_playlistId;
}
QVariantMap PlaylistItem::toMap() const
{
    QVariantMap map;
    if(m_albumId != -1) map.insert("albumid", m_albumId);
    if(m_artistId != -1) map.insert("artistid", m_artistId);
    map.insert("file", m_file);
    if(m_genreId != -1) map.insert("genreid", m_genreId);

    QVariantMap plist;
    plist.insert("id", "testlist");
    map.insert("playlist", plist);
    return map;
}

}
