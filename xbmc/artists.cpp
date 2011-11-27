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

#include "artists.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "albums.h"
#include "audioplaylistitem.h"
#include "audioplayer.h"
#include "playlist.h"

Artists::Artists(XbmcModel *parent) :
    XbmcLibrary(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    QVariantMap params;

    QVariantMap sort;
    sort.insert("ignorearticle", true);
    sort.insert("method", "label");
    params.insert("sort", sort);

    QVariantList properties;
    properties.append("thumbnail");
    params.insert("properties", properties);


    m_requestList.insert(XbmcConnection::sendCommand("AudioLibrary.GetArtists", params), RequestList);
}

Artists::~Artists()
{
}

void Artists::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("artistid", m_list.at(index)->data(RoleArtistId).toInt());

    QVariantList properties;
    properties.append("instrument");
    properties.append("style");
    properties.append("mood");
    properties.append("born");
    properties.append("formed");
    properties.append("description");
    properties.append("genre");
    properties.append("died");
    properties.append("disbanded");
//    properties.append("yearsactive");
//    properties.append("musicbrainzartistid");
//    properties.append("fanart");
//    properties.append("thumbnail");

    params.insert("properties", properties);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    int id = XbmcConnection::sendCommand("AudioLibrary.GetArtistDetails", params);
    m_requestList.insert(id, RequestDetails);
    m_detailsRequestMap.insert(id, index);
}


void Artists::responseReceived(int id, const QVariantMap &rsp)
{
    if(!m_requestList.contains(id)) {
        return;
    }

    switch(m_requestList.value(id)) {
    case RequestList: {
        setBusy(false);
        QList<XbmcModelItem*> list;
        qDebug() << "got artists:" << rsp.value("result");
        QVariantList responseList = rsp.value("result").toMap().value("artists").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            LibraryItem *item = new LibraryItem();
            item->setTitle(itemMap.value("label").toString());
            item->setFileName("directory");
            item->setArtistId(itemMap.value("artistid").toInt());
            item->setThumbnail(itemMap.value("thumbnail").toString());
            item->setIgnoreArticle(true);
            item->setFileType("directory");
            item->setPlayable(true);
            list.append(item);
        }
        beginInsertRows(QModelIndex(), 0, list.count() - 1);
        m_list = list;
        endInsertRows();
        emit layoutChanged();
        }
        break;
    case RequestDetails:
        qDebug() << "got item details:" << rsp;
        LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(m_detailsRequestMap.value(id)));
        QVariantMap details = rsp.value("result").toMap().value("artistdetails").toMap();
        item->setDescription(details.value("description").toString());
        item->setInstrument(details.value("instrument").toString());
        item->setStyle(details.value("style").toString());
        item->setMood(details.value("mood").toString());
        item->setBorn(details.value("born").toString());
        item->setFormed(details.value("formed").toString());
        item->setGenre(details.value("genre").toString());
        item->setDied(details.value("died").toString());
        item->setDisbanded(details.value("disbanded").toString());
        emit dataChanged(index(m_detailsRequestMap.value(id), 0, QModelIndex()), index(m_detailsRequestMap.value(id), 0, QModelIndex()));
    }
}

XbmcModel *Artists::enterItem(int index)
{
    return new Albums(m_list.at(index)->data(RoleArtistId).toInt(), this);
}

void Artists::playItem(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_list.at(index)->data(RoleArtistId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->clear();
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
    Xbmc::instance()->audioPlayer()->playItem(0);
}

void Artists::addToPlaylist(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_list.at(index)->data(RoleArtistId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Artists::title() const
{
    return "Artists";
}
