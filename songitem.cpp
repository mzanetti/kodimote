#include "songitem.h"

namespace Xbmc
{

SongItem::SongItem(int songid, const QString &label):
    m_songId(songid),
    m_label(label)
{
}

void SongItem::setSongId(int songid)
{
    m_songId = songid;
}

void SongItem::setFile(const QString &file)
{
    m_file = file;
}

void SongItem::setLabel(const QString &label)
{
    m_label = label;
}

void SongItem::setTitle(const QString &title)
{
    m_title = title;
}

void SongItem::setArtist(const QString &artist)
{
    m_artist = artist;
}

void SongItem::setAlbum(const QString &album)
{
    m_album = album;
}

void SongItem::setThumbnail(const QString &thumbnail)
{
    m_thumbnail = thumbnail;
}

void SongItem::setFanart(const QString &fanart)
{
    m_fanart = fanart;
}

int SongItem::songId() const
{
    return m_songId;
}

QString SongItem::file() const
{
    return m_file;
}

QString SongItem::label() const
{
    return m_label;
}

QString SongItem::title() const
{
    return m_title;
}

QString SongItem::artist() const
{
    return m_artist;
}

QString SongItem::album() const
{
    return m_album;
}

QString SongItem::thumbnail() const
{
    return m_thumbnail;
}

QString SongItem::fanart() const
{
    return m_fanart;
}

}
