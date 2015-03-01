/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
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
#include "kodiconnection.h"
#include "kodebug.h"

Playlist::Playlist(Player *parent) :
    KodiModel(parent),
    m_currentItem(-1),
    m_player(parent)
{
    connect(KodiConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
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

    KodiConnection::sendCommand("Playlist.Add", params);

    refresh();
}

void Playlist::removeItem(int index)
{
    QVariantMap params;
    params.insert("position", index);
    params.insert("playlistid", playlistId());
    KodiConnection::sendCommand("Playlist.Remove", params);

    refresh();
}

void Playlist::clear()
{
    QVariantMap params;
    params.insert("playlistid", playlistId());
    KodiConnection::sendCommand("Playlist.Clear", params);
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
    koDebug(XDAREA_PLAYLIST) << "setting current index to" << index;
    m_currentItem = index;
    queryItemData(m_currentItem);
    emit currentChanged();
}
