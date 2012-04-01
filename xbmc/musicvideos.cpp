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

#include "musicvideos.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"

MusicVideos::MusicVideos(XbmcModel *parent) :
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

    m_requestList.insert(XbmcConnection::sendCommand("VideoLibrary.GetMusicVideos", params), RequestList);
}

void MusicVideos::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("musicvideoid", m_list.at(index)->data(RoleMusicVideoId).toInt());

    QVariantList properties;
//    properties.append("title");
//    properties.append("playcount");
    properties.append("runtime");
//    properties.append("director");
//    properties.append("studio");
    properties.append("year");
    properties.append("plot");
//    properties.append("album");
//    properties.append("artist");
    properties.append("genre");
//    properties.append("track");
//    properties.append("streamdetails");
//    properties.append("lastplayed");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("resume");

    params.insert("properties", properties);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    int id = XbmcConnection::sendCommand("VideoLibrary.GetMusicVideoDetails", params);
    m_requestList.insert(id, RequestDetails);
    m_detailsRequestMap.insert(id, index);
}


void MusicVideos::responseReceived(int id, const QVariantMap &rsp)
{
    if(!m_requestList.contains(id)) {
        return;
    }

    switch(m_requestList.value(id)) {
    case RequestList: {
        setBusy(false);
        QList<XbmcModelItem*> list;
        qDebug() << "got MusicCideos:" << rsp.value("result");
        QVariantList responseList = rsp.value("result").toMap().value("musicvideos").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            LibraryItem *item = new LibraryItem();
            item->setTitle(itemMap.value("label").toString());
            item->setMusicvideoId(itemMap.value("musicvideoid").toInt());
            item->setThumbnail(itemMap.value("fanart").toString());
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
        QVariantMap details = rsp.value("result").toMap().value("musicvideodetails").toMap();
        //item->setRuntime(details.value("runtime").toInt());
        item->setYear(details.value("year").toString());
        item->setPlot(details.value("plot").toString());
        item->setGenre(details.value("genre").toString());
        emit dataChanged(index(m_detailsRequestMap.value(id), 0, QModelIndex()), index(m_detailsRequestMap.value(id), 0, QModelIndex()));
    }
}

XbmcModel *MusicVideos::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter MusicVideo. Use playItem() to play it";
    return 0;
}

void MusicVideos::playItem(int index)
{
    qDebug() << "should play item" << index << "musicvideoid is" << m_list.at(index)->data(RoleMusicVideoId).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item;
    item.setMusicVideoId(m_list.at(index)->data(RoleMusicVideoId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playItem(0);
}

void MusicVideos::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setMusicVideoId(m_list.at(row)->data(RoleMusicVideoId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString MusicVideos::title() const
{
    return tr("Music Videos");
}
