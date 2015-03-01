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

#include "channels.h"

#include "kodi.h"
#include "kodiconnection.h"
#include "channelitem.h"
#include "videoplaylistitem.h"
#include "videoplayer.h"
#include "playlist.h"

Channels::Channels(int channelgroupid, KodiModel *parent) :
    KodiLibrary(parent),
    m_channelgroupid(channelgroupid)
{
}

QString Channels::title() const
{
    return tr("TV Channels");
}

void Channels::refresh()
{
    QVariantMap params;
    params.insert("channelgroupid", m_channelgroupid);

    QVariantList properties;
    properties.append("thumbnail");

    params.insert("properties", properties);

    KodiConnection::sendCommand("PVR.GetChannels", params, this, "listReceived");
}

KodiModel *Channels::enterItem(int index)
{
    Q_UNUSED(index);

    qDebug() << "cannot enter channel";
    return 0;
}

void Channels::playItem(int index, bool resume)
{
    Q_UNUSED(index);

    VideoPlaylistItem item;
    item.setChannelId(m_list.at(index)->data(RoleChannelId).toInt());
    Kodi::instance()->videoPlayer()->open(item, resume);
}

void Channels::addToPlaylist(int index)
{
    Q_UNUSED(index);
}

QHash<int, QByteArray> Channels::roleNames() const
{
    QHash<int, QByteArray> roles = KodiLibrary::roleNames();
    roles.insert(Qt::UserRole + 2000, "progressPercentage");
    return roles;
}

QVariant Channels::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole + 2000) {
        return static_cast<ChannelItem*>(m_list.at(index.row()))->progressPercent();
    }
    return KodiLibrary::data(index, role);
}

void Channels::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("channelid", m_list.at(index)->data(RoleChannelId).toInt());

    QVariantList properties;
    properties.append("thumbnail");
    properties.append("channeltype");
    properties.append("hidden");
    properties.append("locked");
    properties.append("channel");
    properties.append("lastplayed");

    params.insert("properties", properties);

    int id = KodiConnection::sendCommand("PVR.GetChannelDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Channels::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got channelgroups:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("channels").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        ChannelItem *item = new ChannelItem(itemMap.value("label").toString(), " ", this);

        item->setChannelId(itemMap.value("channelid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());

        item->setFileType("file");
        item->setIgnoreArticle(ignoreArticle());
        item->setPlayable(true);
        list.append(item);

        fetchBroadcasts(item->channelId());
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(KodiModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

void Channels::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
//    int id = rsp.value("id").toInt();
//    int row = m_detailsRequestMap.take(id);
//    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
//    QVariantMap details = rsp.value("result").toMap().value("channeldetails").toMap();

    // TODO: fill in details once there is anything interesting

//    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
    qDebug() << "done 2";
}

void Channels::fetchBroadcasts(int channelId, int start, int end)
{
    QVariantMap params;
    params.insert("channelid", channelId);

    QVariantList properties;
    properties.append("title");
    properties.append("starttime");
    properties.append("progresspercentage");
    properties.append("endtime");
    properties.append("thumbnail");
    properties.append("isactive");
    properties.append("hastimer");

    params.insert("properties", properties);

    QVariantMap limits;
    limits.insert("start", start);
    limits.insert("end", end);
    params.insert("limits", limits);

    int id = KodiConnection::sendParallelCommand("PVR.GetBroadcasts", params, this, "broadcastsReceived");
    m_broadcastRequestMap.insert(id, channelId);
}

void Channels::broadcastsReceived(const QVariantMap &rsp)
{
    if (!m_broadcastRequestMap.contains(rsp.value("id").toInt())) {
        return;
    }

    int endItem = rsp.value("result").toMap().value("limits").toMap().value("end").toInt();
    int totalItems = rsp.value("result").toMap().value("limits").toMap().value("total").toInt();

    int channelId = m_broadcastRequestMap.take(rsp.value("id").toInt());
    ChannelItem *item = 0;
    int itemIndex = -1;
    for (int i = 0; i < m_list.count(); ++i) {
        ChannelItem *tmp = qobject_cast<ChannelItem*>(m_list.at(i));
        if (tmp->channelId() == channelId) {
            itemIndex = i;
            item = tmp;
            break;
        }
    }
    if (itemIndex == -1) {
        return; // model probably cleared since the request
    }

    QVariantList broadcasts = rsp.value("result").toMap().value("broadcasts").toList();

    QDateTime nowTime = QDateTime::currentDateTime();

    bool foundCurrent = false;
    foreach (const QVariant &broadcast, broadcasts) {
        QDateTime startTime = broadcast.toMap().value("starttime").toDateTime();
        startTime.setTimeSpec(Qt::UTC);

        QDateTime endTime = broadcast.toMap().value("endtime").toDateTime();
        endTime.setTimeSpec(Qt::UTC);

        QString title = broadcast.toMap().value("label").toString();

        Broadcast b;
        b.m_title = title;
        b.m_startTime = startTime;
        b.m_endTime = endTime;
        b.m_isActive = broadcast.toMap().value("isactive").toBool();
        b.m_hasTimer = broadcast.toMap().value("hastimer").toBool();
        b.m_progressPercentage = broadcast.toMap().value("progresspercentage").toInt();

        item->channelBroadcasts()->addBroadcast(b);
        if (startTime < nowTime && nowTime < endTime) {
            item->setSubtitle(title);
            item->setProgressPercentage(b.m_progressPercentage);
            foundCurrent = true;
        }
    }
    if (!foundCurrent && endItem < totalItems) {
        fetchBroadcasts(channelId, endItem, qMin(endItem + 20, totalItems));
    } else {
        emit dataChanged(index(itemIndex, 0, QModelIndex()), index(itemIndex, 0, QModelIndex()));
    }

}
