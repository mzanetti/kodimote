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

void SongItem::setDuration(const QTime &duration)
{
    m_duration = duration;
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

QTime SongItem::duration() const
{
    return m_duration;
}

}
