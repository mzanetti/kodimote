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

PlaylistItem::PlaylistItem(const QString &file, const QString &playlist):
    m_file(file),
    m_playlistId(playlist)
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
        QVariantMap playlist;
        playlist.insert("id", m_playlistId);
        map.insert("playlist", playlist);
    }
    return map;
}
