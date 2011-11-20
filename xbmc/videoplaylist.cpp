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

#include "videoplaylist.h"
#include "xbmcconnection.h"
#include "xdebug.h"

VideoPlaylist::VideoPlaylist(Player *parent):
    Playlist(parent)
{
}

int VideoPlaylist::playlistId() const
{
    return 1;
}

void VideoPlaylist::refresh()
{
    QVariantMap params;
    QVariantList properties;
    properties.append("title");
    params.insert("properties", properties);
    params.insert("playlistid", playlistId());

    int id = XbmcConnection::sendCommand("Playlist.GetItems", params);
    m_requestMap.insert(id, RequestGetItems);
}

void VideoPlaylist::queryItemData(int index)
{
    QVariantMap params;
    QVariantList properties;
    properties.append("title");
    properties.append("season");
    properties.append("showtitle");
    properties.append("fanart");
    properties.append("thumbnail");
    properties.append("runtime");
    properties.append("file");
    properties.append("year");
    properties.append("rating");
    params.insert("properties", properties);

    QVariantMap limits;
    limits.insert("start", index);
    limits.insert("end", index + 1);
    params.insert("limits", limits);
    params.insert("playlistid", playlistId());

    int id = XbmcConnection::sendCommand("Playlist.GetItems", params);
    m_requestMap.insert(id, RequestCurrentData);
}

void VideoPlaylist::responseReveiced(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    xDebug(XDAREA_PLAYLIST) << "VideoPlaylist response:" << response;
    if(response.contains("error")) {
        xDebug(XDAREA_PLAYLIST) << "Failed to fetch itemdata";
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestGetItems: {
//        xDebug(XDAREA_PLAYLIST) << "got GetItems response:" << response;
//        xDebug(XDAREA_PLAYLIST) << "resetting model";
        beginResetModel();
        m_itemList.clear();
        QVariantList responseList = rsp.toMap().value("items").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            VideoPlaylistItem *item = new VideoPlaylistItem();
//            item.setFanart(itemMap.value("fanart").toString());
            item->setLabel(itemMap.value("label").toString());
            item->setDuration(QTime().addSecs(itemMap.value("runtime").toInt()));
//            item.setTitle(itemMap.value("title").toString());
//            item.setArtist(itemMap.value("artist").toString());
//            xDebug(XDAREA_PLAYLIST) << "adding item:" << item.label();
            m_itemList.append(item);
        }
        endResetModel();

        m_currentItem = rsp.toMap().value("state").toMap().value("current").toInt();
        xDebug(XDAREA_PLAYLIST) << "set current to" << m_currentItem;
        queryItemData(m_currentItem);
        emit countChanged();
        emit currentChanged();
        break;
    }
    case RequestCurrentData: {
        if(m_itemList.count() > m_currentItem && m_currentItem > -1) {
            VideoPlaylistItem *item = m_itemList.at(m_currentItem);
            QVariantList responseList = rsp.toMap().value("items").toList();
            QVariantMap itemMap = responseList.first().toMap();
    //            item.setFanart(itemMap.value("fanart").toString());
            item->setDuration(QTime().addSecs(itemMap.value("runtime").toInt() * 60));
            item->setLabel(itemMap.value("label").toString());
            item->setFile(itemMap.value("file").toString());
            item->setTitle(itemMap.value("title").toString());
            item->setType(itemMap.value("type").toString());
            item->setTvShow(itemMap.value("showtitle").toString());
            item->setSeason(itemMap.value("season").toString());
            item->setFanart(itemMap.value("fanart").toString());
            item->setThumbnail(itemMap.value("thumbnail").toString());
            item->setYear(itemMap.value("year").toString());
            item->setRating(itemMap.value("rating").toString());
            emit currentChanged();
            break;
        }
        }
    }
}

QVariant VideoPlaylist::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_itemList.at(index.row())->label();
    case Qt::UserRole+1:
        return "file";
//    case Qt::UserRole+2:
//        return m_itemList.at(index.row()).artist() + " - " + m_itemList.at(index.row()).album();
    case Qt::UserRole+3:
        return m_itemList.at(index.row())->duration().toString("mm:ss");
    }
    return QVariant();
}

int VideoPlaylist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_itemList.count();
}

PlaylistItem* VideoPlaylist::at(int index) const
{
    return m_itemList.at(index);
}

QString VideoPlaylist::title() const
{
    return "Now Playing - Videos";
}
