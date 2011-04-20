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

namespace Xbmc
{

Playlist::Playlist(QObject *parent) :
    QAbstractItemModel(parent),
    m_currentSong(-1),
    m_shuffle(false) //TODO: query player for state as soon as API supports it
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReveiced(int,QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    staticMetaObject.invokeMethod(this, "refresh", Qt::QueuedConnection);

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "duration");
    setRoleNames(roleNames);
}

void Playlist::refresh()
{
    QVariantMap params;
    QVariantList fields;
//    fields.append("title");
    fields.append("duration");
    params.insert("fields", fields);

    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems", params);
    m_requestMap.insert(id, RequestGetItems);
}

void Playlist::queryItemData(int index)
{
    QVariantMap params;
    QVariantList fields;
    fields.append("title");
    fields.append("artist");
    fields.append("album");
    fields.append("fanart");
    fields.append("thumbnail");
    fields.append("duration");
    params.insert("fields", fields);

    QVariantMap limits;
    limits.insert("start", index);
    limits.insert("end", index + 1);
    params.insert("limits", limits);

    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems", params);
    m_requestMap.insert(id, RequestCurrentData);
}

void Playlist::clear()
{
    beginResetModel();
    m_itemList.clear();
    XbmcConnection::sendCommand("AudioPlaylist.Clear");
    endResetModel();
}

void Playlist::addItems(int artistId, int albumId)
{

    PlaylistItem pItem;
    pItem.setArtistId(artistId);
    pItem.setAlbumId(albumId);
    QVariantMap item;
    item.insert("item", pItem.toMap());

    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
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
}

void Playlist::responseReveiced(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestGetItems: {
//        qDebug() << "got GetItems response:" << response;
//        qDebug() << "resetting model";
        beginResetModel();
        m_itemList.clear();
        QVariantList responseList = rsp.toMap().value("items").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            SongItem item;
//            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setDuration(QTime().addSecs(itemMap.value("duration").toInt()));
//            item.setTitle(itemMap.value("title").toString());
//            item.setArtist(itemMap.value("artist").toString());
//            qDebug() << "adding item:" << item.label();
            m_itemList.append(item);
        }
        endResetModel();
        m_currentSong = rsp.toMap().value("state").toMap().value("current").toInt();
        qDebug() << "set current to" << m_currentSong;
        queryItemData(m_currentSong);
        emit countChanged();
        emit currentChanged();
        break;
    }
    case RequestCurrentData: {
        if(m_itemList.count() > m_currentSong && m_currentSong > -1) {
            SongItem item = m_itemList.at(m_currentSong);
            QVariantList responseList = rsp.toMap().value("items").toList();
            QVariantMap itemMap = responseList.first().toMap();
    //            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setTitle(itemMap.value("title").toString());
            item.setArtist(itemMap.value("artist").toString());
            item.setAlbum(itemMap.value("album").toString());
            item.setFanart(itemMap.value("fanart").toString());
            item.setThumbnail(itemMap.value("thumbnail").toString());
            m_itemList.replace(m_currentSong, item);
            emit currentChanged();
            break;
        }
        }
    }
}

void Playlist::receivedAnnouncement(const QVariantMap &map)
{
    if(map.value("method").toString() == "Player.QueueNextItem") {
    } else if(map.value("method").toString() == "Player.PlaybackStarted") {
    }
}

QModelIndex Playlist::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex Playlist::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int Playlist::rowCount(const QModelIndex &parent) const
{
    return m_itemList.count();
}

int Playlist::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Playlist::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_itemList.at(index.row()).label();
    case Qt::UserRole:
        return m_itemList.at(index.row()).duration().toString("mm:ss");
    }
    return QVariant();
}

SongItem Playlist::at(int index)
{
    return m_itemList.at(index);
}

void Playlist::playItem(int index)
{
    QVariantMap map;
    map.insert("item", index);
    XbmcConnection::sendCommand(namespaceString() + ".Play", map);
    m_currentSong = index;
    qDebug() << "(2)settings current to" << m_currentSong;
    emit currentChanged();
}

void Playlist::skipNext()
{
    if(m_currentSong < m_itemList.count() - 1) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipNext");
        m_currentSong++;
        qDebug() << "(3)settings current to" << m_currentSong;
        emit currentChanged();
    }
}

void Playlist::skipPrevious()
{
    if(m_currentSong > 0) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipPrevious");
        m_currentSong--;
        qDebug() << "(4)settings current to" << m_currentSong;
        emit currentChanged();
    }
}

QString Playlist::currentLabel() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).label();
}

QString Playlist::currentTitle() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).title();
}

QString Playlist::currentArtist() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).artist();
}
QString Playlist::currentFanart() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).fanart();
}

QString Playlist::currentThumbnail() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).thumbnail();
}

QString Playlist::currentAlbum() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).album();
}

int Playlist::currentTrackNumber() const
{
    return m_currentSong + 1;
}

int Playlist::count() const
{
    return m_itemList.count();
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

SongItem Playlist::currentItem() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return SongItem();
    }
    return m_itemList.at(m_currentSong);
}

}
