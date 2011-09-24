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

PlaylistItem::PlaylistItem(QObject *parent):
    QObject(parent)
{
}

PlaylistItem::~PlaylistItem()
{

}

QString PlaylistItem::file() const
{
    return m_file;
}

void PlaylistItem::setFile(const QString &file)
{
    m_file = file;
}

QString PlaylistItem::directory() const
{
    return m_directory;
}

void PlaylistItem::setDirectory(const QString &directory)
{
    m_directory = directory;
}

QString PlaylistItem::playlistId() const
{
    return m_playlistId;
}

void PlaylistItem::setPlayList(const QString &playlist)
{
    m_playlistId = playlist;
}

QVariantMap PlaylistItem::toMap() const
{
    QVariantMap map;
    if(!m_file.isEmpty()) {
        map.insert("file", m_file);
    }

    if(!m_playlistId.isEmpty()) {
        map.insert("playlistid", m_playlistId);
    }

    if(!m_directory.isEmpty()) {
        map.insert("directory", m_directory);
    }
    return map;
}

void PlaylistItem::setLabel(const QString &label)
{
    m_label = label;
    emit labelChanged();
}

QString PlaylistItem::label() const
{
    return m_label;
}

void PlaylistItem::setDuration(const QTime &duration)
{
    m_duration = duration;
    emit durationChanged();
}

QTime PlaylistItem::duration() const
{
    return m_duration;
}

QString PlaylistItem::durationString() const
{
    if(m_duration.hour() > 0) {
        return m_duration.toString("hh:mm:ss");
    }
    return m_duration.toString("mm:ss");
}

void PlaylistItem::setTitle(const QString &title)
{
    m_title = title;
    emit titleChanged();
}

QString PlaylistItem::title() const
{
    return m_title;
}

void PlaylistItem::setFanart(const QString &fanart)
{
    m_fanart = fanart;
    emit fanartChanged();
}

QString PlaylistItem::fanart() const
{
    return m_fanart;
}

void PlaylistItem::setThumbnail(const QString &thumbnail)
{
    m_thumbnail = thumbnail;
    emit thumbnailChanged();
}

QString PlaylistItem::thumbnail() const
{
    return m_thumbnail;
}
