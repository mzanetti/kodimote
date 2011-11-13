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

TvShows::TvShows(XbmcModel *parent) :
    XbmcLibrary(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    QVariantMap params;
    QVariantList properties;
    properties.append("fanart");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    m_request = XbmcConnection::sendCommand("VideoLibrary.GetTVShows", params);
}

TvShows::~TvShows()
{
}

void TvShows::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<XbmcModelItem*> list;
    qDebug() << "got TvShows:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("tvshows").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setTvshowId(itemMap.value("tvshowid").toInt());
        item->setThumbnail(itemMap.value("fanart").toString());
        item->setIgnoreArticle(true);
        item->setFileType("directory");
        item->setPlayable(false);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
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
    return "TV Shows";
}
