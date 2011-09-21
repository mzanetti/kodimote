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
    QVariantMap params;
    params.insert("item", item.toMap());
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Add", params);

    refresh();
}

void Playlist::removeItem(int index)
{
    QVariantMap params;
    params.insert("item", index);
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
    QVariantMap params;
    params.insert("item", index);
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Play", params);

    m_currentItem = index;
    qDebug() << "setting current of" << playlistId() << "to" << m_currentItem;
    emit currentChanged();
}

void Playlist::skipPrevious()
{
    if(m_currentItem > 0) {
        QVariantMap params;
        params.insert("playlistid", playlistId());

        XbmcConnection::sendCommand("Playlist.SkipPrevious", params);
        m_currentItem--;
        qDebug() << "(4)settings current to" << m_currentItem;
        emit currentChanged();
    }
}

void Playlist::skipNext()
{
    if(m_currentItem < count() - 1) {
        QVariantMap params;
        params.insert("playlistid", playlistId());

        XbmcConnection::sendCommand("Playlist.SkipNext", params);
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
    QVariantMap params;
    params.insert("playlistid", playlistId());

    if(shuffle) {
        XbmcConnection::sendCommand("Playlist.Shuffle", params);
    } else {
        XbmcConnection::sendCommand("Playlist.UnShuffle", params);
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
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.Repeat", params);

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
