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

#include "tvshows.h"
#include "seasons.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"

TvShows::TvShows(XbmcModel *parent) :
    XbmcLibrary(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    QVariantMap params;
    QVariantList properties;
    properties.append("fanart");
    properties.append("playcount");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    m_requestList.insert(XbmcConnection::sendCommand("VideoLibrary.GetTVShows", params), RequestList);
}

TvShows::~TvShows()
{
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

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    int id = XbmcConnection::sendCommand("VideoLibrary.GetTVShowDetails", params);
    m_requestList.insert(id, RequestDetails);
    m_detailsRequestMap.insert(id, index);
}

void TvShows::responseReceived(int id, const QVariantMap &rsp)
{
    if(!m_requestList.contains(id)) {
        return;
    }

    switch(m_requestList.value(id)) {
    case RequestList: {
        setBusy(false);
        QList<XbmcModelItem*> list;
        qDebug() << "got TvShows:" << rsp.value("result");
        QVariantList responseList = rsp.value("result").toMap().value("tvshows").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            LibraryItem *item = new LibraryItem();
            item->setTitle(itemMap.value("label").toString());
            item->setTvshowId(itemMap.value("tvshowid").toInt());
            item->setThumbnail(itemMap.value("fanart").toString());
            item->setPlaycount(itemMap.value("playcount").toInt());
            item->setIgnoreArticle(true);
            item->setFileType("directory");
            item->setPlayable(false);
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
        QVariantMap details = rsp.value("result").toMap().value("tvshowdetails").toMap();
        item->setGenre(details.value("genre").toString());
        item->setPlot(details.value("plot").toString());
        item->setRating(details.value("rating").toInt());
        item->setYear(details.value("year").toString());
        item->setFirstAired(details.value("firstaired").toString());
        item->setMpaa(details.value("mpaa").toString());
        item->setCast(details.value("cast").toString());
        emit dataChanged(index(m_detailsRequestMap.value(id), 0, QModelIndex()), index(m_detailsRequestMap.value(id), 0, QModelIndex()));
    }
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
