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

#include "albums.h"
#include "xbmcconnection.h"

#include "songs.h"
#include "audioplaylistitem.h"
#include "xbmc.h"
#include "audioplayer.h"
#include "playlist.h"
#include "libraryitem.h"
#include "xbmcdownload.h"

Albums::Albums(int artistId, XbmcModel *parent) :
    XbmcLibrary(parent),
    m_artistId(artistId)
{
}

Albums::~Albums()
{
    qDebug() << "deleting album model";
}

void Albums::refresh()
{
    QVariantMap params;
    if(m_artistId != -1) {
        QVariantMap filter;
        filter.insert("artistid", m_artistId);
        params.insert("filter", filter);
    }

    QVariantList properties;
    properties.append("artist");
    properties.append("thumbnail");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", ignoreArticle());
    params.insert("sort", sort);
    XbmcConnection::sendCommand("AudioLibrary.GetAlbums", params, this, "listReceived");
}

void Albums::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("albumid", m_list.at(index)->data(RoleAlbumId).toInt());

    QVariantList properties;
//    properties.append("title");
    properties.append("description");
//    properties.append("artist");
    properties.append("genre");
//    properties.append("theme");
//    properties.append("mood");
//    properties.append("style");
//    properties.append("type");
//    properties.append("albumlabel");
    properties.append("rating");
    properties.append("year");
//    properties.append("musicbrainzalbumid");
//    properties.append("musicbrainzalbumartistid");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("artistid");

    params.insert("properties", properties);

    int id = XbmcConnection::sendCommand("AudioLibrary.GetAlbumDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Albums::download(int index, const QString &path)
{
    qDebug() << "Downloading album";
    m_downloadPath = path;
    Songs *downloadModel = new Songs(m_artistId, m_list.at(index)->data(RoleAlbumId).toInt());
    downloadModel->setDeleteAfterDownload(true);
    m_downloadList.append(downloadModel);
    connect(downloadModel, SIGNAL(busyChanged()), SLOT(downloadModelFilled()));
}

void Albums::listReceived(const QVariantMap &rsp)
{
    QList<XbmcModelItem*> list;
    qDebug() << "got albums:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("albums").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setSubtitle(itemMap.value("artist").toString());
        item->setAlbumId(itemMap.value("albumid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setFileType("directory");
        item->setPlayable(true);
        item->setIgnoreArticle(ignoreArticle());

        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
    setBusy(false);
}

void Albums::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(m_detailsRequestMap.value(row)));
    QVariantMap details = rsp.value("result").toMap().value("albumdetails").toMap();
    item->setDescription(details.value("description").toString());
    item->setRating(details.value("rating").toInt());
    item->setGenre(details.value("genre").toString());
    item->setYear(details.value("year").toString());
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

void Albums::downloadModelFilled()
{
    Songs *downloadModel = qobject_cast<Songs*>(sender());
    qDebug() << "starting batch download of album";
    for(int i = 0; i < downloadModel->rowCount(); ++i) {
        downloadModel->download(i, m_downloadPath);
    }

    m_downloadList.removeAll(downloadModel);
    if(deleteAfterDownload() && m_downloadList.isEmpty()) {
        deleteLater();
    }
}

XbmcModel* Albums::enterItem(int index)
{
    return new Songs(m_artistId, m_list.at(index)->data(RoleAlbumId).toInt(), this);
}

void Albums::playItem(int index)
{
    AudioPlaylistItem pItem;
    pItem.setAlbumId(m_list.at(index)->data(RoleAlbumId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->clear();
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
    Xbmc::instance()->audioPlayer()->playItem(0);
}

void Albums::addToPlaylist(int index)
{
    AudioPlaylistItem pItem;
    pItem.setAlbumId(m_list.at(index)->data(RoleAlbumId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Albums::title() const
{
    return tr("Albums");
}
