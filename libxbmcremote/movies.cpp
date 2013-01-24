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

#include "movies.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"
#include "xbmcdownload.h"

Movies::Movies(bool recentlyAdded, XbmcModel *parent) :
    XbmcLibrary(parent),
    m_recentlyAdded(recentlyAdded)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

Movies::~Movies()
{
}

void Movies::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();

    if(method != "VideoLibrary.OnUpdate") {
        return;
    }

    QVariantMap data = map.value("params").toMap().value("data").toMap();

    QVariant playcount = data.value("playcount");
    if(!playcount.isValid() || playcount.toInt() < 0) {
        return;
    }

    QString type = data.value("item").toMap().value("type").toString();
    int id = data.value("item").toMap().value("id").toInt();
    if(type != "movie" || !m_idIndexMapping.contains(id)) {
        return;
    }

    int i = m_idIndexMapping.value(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(i));

    if(playcount.toInt() == item->playcount()) {
        return;
    }


    item->setPlaycount(playcount.toInt());
    dataChanged(index(i, 0, QModelIndex()), index(i, 0, QModelIndex()));
}

void Movies::refresh()
{
    QVariantMap params;
    QVariantList properties;
    properties.append("fanart");
    properties.append("thumbnail");
    properties.append("playcount");
    properties.append("file");
    properties.append("genre");
    properties.append("year");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", ignoreArticle());
    params.insert("sort", sort);

    if (m_recentlyAdded) {
        XbmcConnection::sendCommand("VideoLibrary.GetRecentlyAddedMovies", params, this, "listReceived");
    } else {
        XbmcConnection::sendCommand("VideoLibrary.GetMovies", params, this, "listReceived");
    }
}

void Movies::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("movieid", m_list.at(index)->data(RoleMovieId).toInt());

    QVariantList properties;

//    properties.append("title");
    properties.append("genre");
    properties.append("year");
    properties.append("rating");
    properties.append("director");
//    properties.append("trailer");
    properties.append("tagline");
    properties.append("plot");
//    properties.append("plotoutline");
//    properties.append("originaltitle");
//    properties.append("lastplayed");
//    properties.append("playcount");
//    properties.append("writer");
//    properties.append("studio");
    properties.append("mpaa");
//    properties.append("cast");
//    properties.append("country");
//    properties.append("imdbnumber");
//    properties.append("premiered");
//    properties.append("productioncode");
    properties.append("runtime");
//    properties.append("set");
//    properties.append("showlink");
//    properties.append("streamdetails");
//    properties.append("top250");
//    properties.append("votes");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("sorttitle");
//    properties.append("resume");
//    properties.append("setid");

    params.insert("properties", properties);

    int id = XbmcConnection::sendCommand("VideoLibrary.GetMovieDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Movies::download(int index, const QString &path)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QString destination = path + "/Movies/";
    qDebug() << "should download" << destination;

    XbmcDownload *download = new XbmcDownload();
    download->setDestination(destination);
    download->setIconId("icon-m-content-videos");
    download->setLabel(item->title());

    startDownload(index, download);
}

void Movies::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<XbmcModelItem*> list;
    //qDebug() << "got movies:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("movies").toList();
    int index = 0;
    m_idIndexMapping.clear();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setSubtitle(itemMap.value("genre").toString());
        item->setMovieId(itemMap.value("movieid").toInt());
        item->setYear(itemMap.value("year").toString());
        item->setFanart(itemMap.value("fanart").toString());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setFileName(itemMap.value("file").toString());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("file");
        item->setPlayable(true);
        list.append(item);
        m_idIndexMapping.insert(item->movieId(), index++);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
    emit layoutChanged();
}

void Movies::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("moviedetails").toMap();
    item->setGenre(details.value("genre").toString());
    item->setYear(details.value("year").toString());
    item->setRating(details.value("rating").toInt());
    item->setDirector(details.value("director").toString());
    item->setTagline(details.value("tagline").toString());
    item->setPlot(details.value("plot").toString());
    item->setMpaa(details.value("mpaa").toString());
    item->setDuration(QTime(0, 0, 0).addSecs(details.value("runtime").toDouble()));
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

XbmcModel *Movies::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter Movie. Use playItem() to play it";
    return 0;
}

void Movies::playItem(int index)
{
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item(m_list.at(index)->data(RoleMovieId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playItem(0);
}

void Movies::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setMovieId(m_list.at(row)->data(RoleMovieId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString Movies::title() const
{
    return tr("Movies");
}
