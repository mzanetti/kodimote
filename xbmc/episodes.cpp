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
    if(seasonid != -1) {
        params.insert("season", seasonid);
    }
    QVariantList properties;
    properties.append("showtitle");
    properties.append("episode");
    properties.append("thumbnail");
    properties.append("playcount");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "episode");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_requestList.insert(XbmcConnection::sendCommand("VideoLibrary.GetEpisodes", params), RequestList);
}

void Episodes::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("episodeid", m_list.at(index)->data(RoleEpisodeId).toInt());

    QVariantList properties;
//    properties.append("resume");
//    properties.append("title");

    properties.append("plot");

//    properties.append("votes");
    properties.append("rating");
//    properties.append("writer");
    properties.append("firstaired");
    properties.append("playcount");
    properties.append("runtime");
//    properties.append("director");
//    properties.append("productioncode");
    properties.append("season");
    properties.append("episode");
//    properties.append("originaltitle");
//    properties.append("showtitle");
//    properties.append("cast");
//    properties.append("streamdetails");
//    properties.append("lastplayed");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("resume");
//    properties.append("tvshowid");

    params.insert("properties", properties);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    int id = XbmcConnection::sendCommand("VideoLibrary.GetEpisodeDetails", params);
    m_requestList.insert(id, RequestDetails);
    m_detailsRequestMap.insert(id, index);
}

void Episodes::responseReceived(int id, const QVariantMap &rsp)
{
    if(!m_requestList.contains(id)) {
        return;
    }

    switch(m_requestList.value(id)) {
    case RequestList: {
        setBusy(false);
        QList<XbmcModelItem*> list;
        qDebug() << "got Episodes:" << rsp.value("result");
        QVariantList responseList = rsp.value("result").toMap().value("episodes").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            LibraryItem *item = new LibraryItem();
            item->setTitle(itemMap.value("episode").toString() + ". " + itemMap.value("label").toString());
            //        item->setData(itemMap.value("showtitle").toString() + " - " + itemMap.value("season").toString(), Qt::UserRole+2);
            item->setSubtitle(itemMap.value("showtitle").toString() + " - " + m_seasonString);
            item->setEpisodeId(itemMap.value("episodeid").toInt());
            item->setThumbnail(itemMap.value("thumbnail").toString());
            item->setPlaycount(itemMap.value("playcount").toInt());
            item->setIgnoreArticle(false);
            item->setFileType("file");
            item->setPlayable(true);
            list.append(item);
        }
        beginInsertRows(QModelIndex(), 0, list.count() - 1);
        m_list = list;
        endInsertRows();
        }
        break;
    case RequestDetails:
        qDebug() << "got item details:" << rsp;
        LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(m_detailsRequestMap.value(id)));
        QVariantMap details = rsp.value("result").toMap().value("episodedetails").toMap();
        item->setPlot(details.value("plot").toString());
        item->setRating(details.value("rating").toInt());
        item->setSeason(details.value("season").toInt());
        item->setEpisode(details.value("episode").toInt());
        item->setFirstAired(details.value("firstaired").toString());
        emit dataChanged(index(m_detailsRequestMap.value(id), 0, QModelIndex()), index(m_detailsRequestMap.value(id), 0, QModelIndex()));
    }
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
//    pItem.setTvShowId(m_tvshowid);
//    pItem.setSeasonId(m_seasonid);
    pItem.setEpisodeId(index(row, 0, QModelIndex()).data(RoleEpisodeId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString Episodes::title() const
{
    return "Episodes";
}
