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

#include "audioplaylist.h"

#include "kodiconnection.h"
#include "kodebug.h"

AudioPlaylist::AudioPlaylist(Player *parent):
    Playlist(parent)
{
}

int AudioPlaylist::playlistId() const
{
    return 0;
}

int AudioPlaylist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_itemList.count();
}

void AudioPlaylist::refresh()
{
    koDebug(XDAREA_PLAYLIST) << "Refreshing playlist" << playlistId();
    QVariantMap params;
    QVariantList properties;
    properties.append("duration");
    properties.append("artist");
    properties.append("album");
    properties.append("thumbnail");
    params.insert("properties", properties);

    params.insert("playlistid", playlistId());

    KodiConnection::sendCommand("Playlist.GetItems", params, this, "itemsReceived");
}

void AudioPlaylist::queryItemData(int index)
{
    QVariantMap params;
    QVariantList properties;
    properties.append("title");
    properties.append("artist");
    properties.append("album");
    properties.append("fanart");
    properties.append("thumbnail");
    properties.append("duration");
    params.insert("properties", properties);

    QVariantMap limits;
    limits.insert("start", index);
    limits.insert("end", index + 1);
    params.insert("limits", limits);
    params.insert("playlistid", playlistId());

    koDebug(XDAREA_PLAYLIST) << "Gettin current item data" << params;
    KodiConnection::sendCommand("Playlist.GetItems", params, this, "currentDataReceived");
}


void AudioPlaylist::itemsReceived(const QVariantMap &rsp)
{
//    koDebug(XDAREA_PLAYLIST) << "AudioPlaylist response:" << rsp;

    koDebug(XDAREA_PLAYLIST) << "got GetItems response:" << rsp;
    QVariantList responseList = rsp.value("result").toMap().value("items").toList();
    bool modelResetted = false;
    if(responseList.count() != m_itemList.count()) {
        beginResetModel();
        modelResetted = true;
    }
    while(!m_itemList.isEmpty()){
        delete m_itemList.takeFirst();
    }
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        AudioPlaylistItem *item = new AudioPlaylistItem();
        item->setLabel(itemMap.value("label").toString());
        item->setDuration(QTime(0, 0, 0).addSecs(itemMap.value("duration").toInt()));
        item->setArtist(itemMap.value("artist").toString());
        item->setAlbum(itemMap.value("album").toString());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        m_itemList.append(item);
    }
    if(modelResetted) {
        endResetModel();
    } else {
        emit dataChanged(index(0, 0, QModelIndex()), index(m_itemList.count() - 1, 0, QModelIndex()));
    }
    emit countChanged();
}

void AudioPlaylist::currentDataReceived(const QVariantMap &rsp)
{
    koDebug(XDAREA_PLAYLIST) << "Current item data response" << rsp;
    if(m_itemList.count() > m_currentItem && m_currentItem > -1) {
        AudioPlaylistItem *item = m_itemList.at(m_currentItem);
        QVariantList responseList = rsp.value("result").toMap().value("items").toList();
        QVariantMap itemMap = responseList.first().toMap();
        item->setLabel(itemMap.value("label").toString());
        item->setTitle(itemMap.value("title").toString());
        item->setArtist(itemMap.value("artist").toString());
        item->setAlbum(itemMap.value("album").toString());
        item->setFanart(itemMap.value("fanart").toString());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        emit currentChanged();
    }
}

QVariant AudioPlaylist::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_itemList.at(index.row())->label();
    case Qt::UserRole+1:
        return "file";
    case Qt::UserRole+2:
        return m_itemList.at(index.row())->artist() + " - " + m_itemList.at(index.row())->album();
    case Qt::UserRole+3:
        return m_itemList.at(index.row())->duration().toString("mm:ss");
    }
    return QVariant();
}

PlaylistItem* AudioPlaylist::at(int index) const
{
    return m_itemList.at(index);
}

QString AudioPlaylist::title() const
{
    return tr("Now Playing") + " - " + tr("Music");
}
