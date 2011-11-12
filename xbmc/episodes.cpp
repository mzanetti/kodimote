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

#include "episodes.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

Episodes::Episodes(int tvshowid, int seasonid, const QString &seasonString, XbmcModel *parent):
    XbmcLibrary(parent),
    m_tvshowid(tvshowid),
    m_seasonid(seasonid),
    m_seasonString(seasonString)
{
    QVariantMap params;
    if(tvshowid != -1) {
      params.insert("tvshowid", tvshowid);
    }
    if(tvshowid != -1) {
      params.insert("season", seasonid);
    }
    QVariantList properties;
    properties.append("showtitle");
    properties.append("episode");
    properties.append("thumbnail");
//    properties.append("fanart");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "episode");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetEpisodes", params);
}

void Episodes::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got Episodes:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("episodes").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("episode").toString() + ". " + itemMap.value("label").toString());
//        item->setData(itemMap.value("showtitle").toString() + " - " + itemMap.value("season").toString(), Qt::UserRole+2);
        item->setData(itemMap.value("showtitle").toString() + " - " + m_seasonString, RoleSubtitle);
        item->setData(itemMap.value("episodeid").toInt(), RoleEpisodeId);
        item->setData(itemMap.value("thumbnail").toString(), RoleThumbnail);
//        item->setData(itemMap.value("fanart").toString(), RoleThumbnail);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Episodes::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Episodes::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleFileType:
        return "file";
    case RolePlayable:
        return true;
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Episodes::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter Episode. Use playItem() to play it";
    return 0;
}

void Episodes::playItem(int index)
{
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item;
    item.setEpisodeId(m_list.at(index)->data(RoleEpisodeId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playItem(0);
}

void Episodes::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setTvShowId(m_tvshowid);
    pItem.setSeasonId(m_seasonid);
    pItem.setEpisodeId(index(row, 0, QModelIndex()).data(RoleEpisodeId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString Episodes::title() const
{
    return "Episodes";
}
