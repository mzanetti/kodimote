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

#include "playlist.h"

#include "playlistitem.h"
#include "xbmcconnection.h"

Playlist::Playlist(Player *parent) :
    m_currentItem(-1),
    m_player(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    staticMetaObject.invokeMethod(this, "refresh", Qt::QueuedConnection);
}

Player *Playlist::player() const
{
    return m_player;
}

void Playlist::addItems(const PlaylistItem &item)
{
    QVariantMap params;
    params.insert("item", item.toMap());
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Add", params);

    refresh();
}

void Playlist::removeItem(int index)
{
    QVariantMap params;
    params.insert("position", index);
    params.insert("playlistid", playlistId());
    XbmcConnection::sendCommand("Playlist.Remove", params);

    refresh();
}

void Playlist::clear()
{
    QVariantMap params;
    params.insert("playlistid", playlistId());
    XbmcConnection::sendCommand("Playlist.Clear", params);
    refresh();
}

void Playlist::addPlaylist(const QString &playlist)
{
    PlaylistItem pItem;
    pItem.setPlayList(playlist);

    QVariantMap params;
    params.insert("item", pItem.toMap());
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Add", params);
    refresh();
}

void Playlist::addFile(const QString &file)
{
    PlaylistItem pItem;
    pItem.setFile(file);

    QVariantMap params;
    params.insert("item", pItem.toMap());
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Add", params);
    refresh();
}

void Playlist::addDirectory(const QString &dir)
{
    PlaylistItem pItem;
    pItem.setDirectory(dir);

    QVariantMap params;
    params.insert("item", pItem.toMap());
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Add", params);
    refresh();
}

void Playlist::receivedAnnouncement(const QVariantMap &map)
{
    Q_UNUSED(map)
}

int Playlist::currentTrackNumber() const
{
    return m_currentItem + 1;
}

int Playlist::count() const
{
    return rowCount();
}

PlaylistItem* Playlist::currentItem() const
{
    if(m_currentItem == -1 || m_currentItem >= count()) {
        return 0;
    }
    return at(m_currentItem);
}

void Playlist::setCurrentIndex(int index)
{
    m_currentItem = index;
    queryItemData(m_currentItem);
    emit currentChanged();
}
