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

#include "tvshows.h"
#include "seasons.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"

TvShows::TvShows(XbmcModel *parent) :
    XbmcLibrary(parent),
    m_refreshing(false)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

TvShows::~TvShows()
{
}

void TvShows::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();

    if(method != "VideoLibrary.OnUpdate")
        return;

    QVariantMap data = map.value("params").toMap().value("data").toMap();
    QString type = data.value("item").toMap().value("type").toString();
    if(type != "episode") {
        return;
    }

    QVariant playcount = map.value("params").toMap().value("data").toMap().value("playcount");
    if(!playcount.isValid() || playcount.toInt() < 0) {
        return;
    }

    if(m_refreshing) {
        return;
    }

    QVariantMap params;
    QVariantList properties;
    properties.append("playcount");
    params.insert("properties", properties);

    XbmcConnection::sendCommand("VideoLibrary.GetTVShows", params, this, "playcountReceived");
    m_refreshing = true;
}

void TvShows::playcountReceived(const QVariantMap &rsp)
{
    m_refreshing = false;
    QVariantList tvshows = rsp.value("result").toMap().value("tvshows").toList();
    foreach(const QVariant &tvshow, tvshows) {
        QVariantMap tvshowMap = tvshow.toMap();

        int i = m_idIndexMapping.value(tvshowMap.value("tvshowid").toInt());
        LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(i));
        item->setPlaycount(tvshowMap.value("playcount").toInt());
    }
    dataChanged(index(0, 0, QModelIndex()), index(m_list.count() - 1, 0, QModelIndex()));
}

void TvShows::refresh()
{
    QVariantMap params;
    QVariantList properties;
    properties.append("fanart");
    properties.append("playcount");
    properties.append("year");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", ignoreArticle());
    params.insert("sort", sort);

    XbmcConnection::sendCommand("VideoLibrary.GetTVShows", params, this, "showsReceived");
    m_refreshing = true;
}

void TvShows::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("tvshowid", m_list.at(index)->data(RoleTvShowId).toInt());

    QVariantList properties;
//    properties.append("title");
    properties.append("genre");
    properties.append("year");
    properties.append("rating");
    properties.append("plot");
//    properties.append("studio");
    properties.append("mpaa");
    properties.append("cast");
//    properties.append("playcount");
//    properties.append("episode");
//    properties.append("imdbnumber");
//    properties.append("premiered");
//    properties.append("votes");
//    properties.append("lastplayed");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("originaltitle");
//    properties.append("sorttitle");
//    properties.append("episodeguide");

    params.insert("properties", properties);

    int id = XbmcConnection::sendCommand("VideoLibrary.GetTVShowDetails", params, this, "showDetailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void TvShows::showsReceived(const QVariantMap &rsp)
{
    m_refreshing = false;
    setBusy(false);
    QList<XbmcModelItem*> list;
    qDebug() << "got TvShows:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("tvshows").toList();
    int index = 0;
    m_idIndexMapping.clear();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        item->setTvshowId(itemMap.value("tvshowid").toInt());
        item->setThumbnail(itemMap.value("fanart").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("directory");
        item->setPlayable(false);
        item->setYear(itemMap.value("year").toString());
        list.append(item);
        m_idIndexMapping.insert(item->tvshowId(), index++);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

void TvShows::showDetailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("tvshowdetails").toMap();
    item->setGenre(details.value("genre").toString());
    item->setPlot(details.value("plot").toString());
    item->setRating(details.value("rating").toInt());
    item->setYear(details.value("year").toString());
    item->setFirstAired(details.value("firstaired").toString());
    item->setMpaa(details.value("mpaa").toString());
    item->setCast(details.value("cast").toString());
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

XbmcModel *TvShows::enterItem(int index)
{
    return new Seasons(m_list.at(index)->data(RoleTvShowId).toInt(), this);
}

void TvShows::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "TvShows: playing whole tvshow not supported by xbmc";
}

void TvShows::addToPlaylist(int row)
{
    Q_UNUSED(row)
    qDebug() << "TvShows: playing whole tvshow not supported by xbmc";
}

QString TvShows::title() const
{
    return tr("TV Shows");
}
