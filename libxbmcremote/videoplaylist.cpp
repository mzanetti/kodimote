/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
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
    properties.append("runtime");
    params.insert("properties", properties);
    params.insert("playlistid", playlistId());

    XbmcConnection::sendCommand("Playlist.GetItems", params, this, "itemsReceived");
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

    XbmcConnection::sendCommand("Playlist.GetItems", params, this, "currentDataReceived");
}

void VideoPlaylist::itemsReceived(const QVariantMap &rsp)
{
    xDebug(XDAREA_PLAYLIST) << "VideoPlaylist response:" << rsp;
    if(rsp.contains("error")) {
        xDebug(XDAREA_PLAYLIST) << "Failed to fetch itemdata";
        return;
    }

    QVariant result = rsp.value("result");

//    xDebug(XDAREA_PLAYLIST) << "got GetItems response:" << response;
//    xDebug(XDAREA_PLAYLIST) << "resetting model";
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
        VideoPlaylistItem *item = new VideoPlaylistItem();
        item->setLabel(itemMap.value("label").toString());
        item->setDuration(QTime(0, 0, 0).addSecs(itemMap.value("runtime").toDouble()));
//        item.setTitle(itemMap.value("title").toString());
//        item.setArtist(itemMap.value("artist").toString());
        xDebug(XDAREA_PLAYLIST) << "adding item:" << item->label() << item->fanart();
        m_itemList.append(item);
    }
    if(modelResetted) {
        endResetModel();
    } else {
        emit dataChanged(index(0, 0, QModelIndex()), index(m_itemList.count() - 1, 0, QModelIndex()));
    }
    emit countChanged();
}

void VideoPlaylist::currentDataReceived(const QVariantMap &rsp)
{
    if(m_itemList.count() > m_currentItem && m_currentItem > -1) {
        VideoPlaylistItem *item = m_itemList.at(m_currentItem);
        QVariantList responseList = rsp.value("result").toMap().value("items").toList();
        QVariantMap itemMap = responseList.first().toMap();
//        item.setFanart(itemMap.value("fanart").toString());
        item->setDuration(QTime(0, 0, 0).addSecs(itemMap.value("runtime").toDouble()));
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
    }
}

QVariant VideoPlaylist::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleTitle:
        return m_itemList.at(index.row())->label();
    case RoleFileType:
        return "file";
//    case RoleSubTitle:
//        return m_itemList.at(index.row()).artist() + " - " + m_itemList.at(index.row()).album();
    case RoleDuration: {
        QTime duration = m_itemList.at(index.row())->duration();
        if(duration.hour() > 0) {
            return m_itemList.at(index.row())->duration().toString("hh:mm:ss");
        } else {
            return m_itemList.at(index.row())->duration().toString("mm:ss");
        }
    }
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
    return tr("Now Playing") + " - " + tr("Videos");
}
