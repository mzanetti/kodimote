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

#include "artists.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "albums.h"
#include "audioplaylistitem.h"
#include "audioplayer.h"
#include "playlist.h"
#include "libraryitem.h"

Artists::Artists(XbmcModel *parent) :
    XbmcLibrary(parent)
{
}

void Artists::refresh()
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("ignorearticle", ignoreArticle());
    sort.insert("method", "label");
    params.insert("sort", sort);

    QVariantList properties;
    properties.append("thumbnail");
    params.insert("properties", properties);

    XbmcConnection::sendCommand("AudioLibrary.GetArtists", params, this, "listReceived");
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

    int id = XbmcConnection::sendCommand("AudioLibrary.GetArtistDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Artists::download(int index, const QString &path)
{
    qDebug() << "Downloading artist";
    m_downloadPath = path;
    Albums *downloadModel = new Albums(m_list.at(index)->data(RoleArtistId).toInt());
    downloadModel->setDeleteAfterDownload(true);
    connect(downloadModel, SIGNAL(busyChanged()), SLOT(downloadModelFilled()));
}

void Artists::listReceived(const QVariantMap &rsp)
{
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
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("directory");
        item->setPlayable(true);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
    emit layoutChanged();
}

void Artists::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
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
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

void Artists::downloadModelFilled()
{
    Albums *albums = qobject_cast<Albums*>(sender());
    qDebug() << "starting batch download of artist";
    for(int i = 0; i < albums->rowCount(); ++i) {
        albums->download(i, m_downloadPath);
    }
    if(deleteAfterDownload()) {
        deleteLater();
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
    return tr("Artists");
}
