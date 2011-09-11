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
    XbmcModel(0),
    m_currentItem(-1),
    m_shuffle(false), //TODO: query player for state as soon as API supports it
    m_player(parent),
    m_repeat(RepeatOne)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReveiced(int,QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    staticMetaObject.invokeMethod(this, "refresh", Qt::QueuedConnection);
}

Player *Playlist::player() const
{
    return m_player;
}

void Playlist::addItems(const PlaylistItem &item)
{
    QVariantMap itemMap;
    itemMap.insert("item", item.toMap());

    XbmcConnection::sendCommand(namespaceString() + ".Add", itemMap);

    refresh();
}

void Playlist::removeItem(int index)
{
    QVariantMap params;
    params.insert("item", index);
    XbmcConnection::sendCommand(namespaceString() + ".Remove", params);

    refresh();
}

void Playlist::clear()
{
    XbmcConnection::sendCommand(namespaceString() + ".Clear");
    refresh();
}

void Playlist::addPlaylist(const QString &playlistId)
{
    PlaylistItem pItem;
    pItem.setPlayList(playlistId);
    QVariantMap item;
    item.insert("item", pItem.toMap());

    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
}

void Playlist::addFile(const QString &file)
{
    PlaylistItem pItem;
    pItem.setFile(file);
    QVariantMap item;
    item.insert("item", pItem.toMap());
    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
    refresh();
}

void Playlist::receivedAnnouncement(const QVariantMap &map)
{
    if(map.value("method").toString() == "Player.QueueNextItem") {
    } else if(map.value("method").toString() == "Player.OnPlay") {
        qDebug() << "current has changed!";
        emit currentChanged();
    }
}

void Playlist::playItem(int index)
{
    QVariantMap map;
    map.insert("item", index);
    XbmcConnection::sendCommand(namespaceString() + ".Play", map);
    m_currentItem = index;
    qDebug() << namespaceString() + "setting current to" << m_currentItem;
    emit currentChanged();
}

void Playlist::skipPrevious()
{
    if(m_currentItem > 0) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipPrevious");
        m_currentItem--;
        qDebug() << "(4)settings current to" << m_currentItem;
        emit currentChanged();
    }
}

void Playlist::skipNext()
{
    if(m_currentItem < count() - 1) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipNext");
        m_currentItem++;
        qDebug() << "(3)settings current to" << m_currentItem;
        emit currentChanged();
    }
}

int Playlist::currentTrackNumber() const
{
    return m_currentItem + 1;
}

int Playlist::count() const
{
    return rowCount();
}

bool Playlist::shuffle() const
{
    return m_shuffle;
}

void Playlist::setShuffle(bool shuffle)
{
    if(shuffle) {
        XbmcConnection::sendCommand(namespaceString() + ".Shuffle");
    } else {
        XbmcConnection::sendCommand(namespaceString() + ".UnShuffle");
    }
    if(m_shuffle != shuffle) {
        m_shuffle = shuffle;
        emit shuffleChanged();
    }
}

void Playlist::setRepeat(Playlist::Repeat repeat)
{
    QVariantMap params;
    switch(repeat) {
    case RepeatNone:
        params.insert("state", "off");
        break;
    case RepeatOne:
        params.insert("state", "one");
        break;
    case RepeatAll:
        params.insert("state", "all");
        break;
    }

    XbmcConnection::sendCommand(namespaceString() + ".Repeat", params);

    if(m_repeat != repeat) {
        m_repeat = repeat;
        emit repeatChanged();
    }
}

Playlist::Repeat Playlist::repeat() const
{
    return m_repeat;
}

PlaylistItem* Playlist::currentItem() const
{
    if(m_currentItem == -1 || m_currentItem >= count()) {
        return 0;
    }
    return at(m_currentItem);
}
