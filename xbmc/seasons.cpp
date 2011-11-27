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

#include "seasons.h"
#include "episodes.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

Seasons::Seasons(int tvshowid, XbmcModel *parent):
    XbmcLibrary(parent),
    m_tvshowid(tvshowid)
{
    QVariantMap params;
    if(tvshowid != -1) {
      params.insert("tvshowid", tvshowid);
    }
    QVariantList properties;
    properties.append("showtitle");
    properties.append("season");
    properties.append("fanart");
    properties.append("playcount");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetSeasons", params);
}

void Seasons::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    setBusy(false);
    QList<XbmcModelItem*> list;
    qDebug() << "got Seasons:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("seasons").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setSubtitle(itemMap.value("showtitle").toString());
        item->setSeasonId(itemMap.value("season").toInt());
        item->setThumbnail(itemMap.value("fanart").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setIgnoreArticle(false);
        item->setFileType("directory");
        item->setPlayable(false);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

XbmcModel *Seasons::enterItem(int index)
{
    return new Episodes(m_tvshowid, m_list.at(index)->data(RoleSeasonId).toInt(), m_list.at(index)->title(), this);
}

void Seasons::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Seasons: playing whole season not supported by xbmc";
}

void Seasons::addToPlaylist(int row)
{
    Q_UNUSED(row)
    qDebug() << "Seasons: playing whole season not supported by xbmc";
}

QString Seasons::title() const
{
    return "Seasons";
}
