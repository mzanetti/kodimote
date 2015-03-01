/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "songs.h"
#include "audioplayer.h"
#include "audioplaylist.h"
#include "audioplaylistitem.h"
#include "kodi.h"
#include "kodiconnection.h"
#include "libraryitem.h"
#include "kodidownload.h"
#include "kodebug.h"

Songs::Songs(int artistid, int albumid, KodiModel *parent):
    KodiLibrary(parent),
    m_artistId(artistid),
    m_albumId(albumid)
{
}

Songs::~Songs()
{
    qDebug() << "deleting songs model";
}

void Songs::refresh(int start, int end)
{
    QVariantMap params;

    if(m_albumId >= 0) {
        QVariantMap filter;
        filter.insert("albumid", m_albumId);
        params.insert("filter", filter);
    }

    QVariantList properties;
    properties.append("artist");
    properties.append("album");
    properties.append("thumbnail");
    properties.append("file");
    properties.append("year");

    params.insert("properties", properties);

    if (m_albumId != KodiModel::ItemIdRecentlyAdded && m_albumId != KodiModel::ItemIdRecentlyPlayed){
        QVariantMap sort;
        if(m_albumId == KodiModel::ItemIdInvalid) {
            sort.insert("method", "label");
        } else {
            sort.insert("method", "track");
        }
        sort.insert("order", "ascending");
        params.insert("sort", sort);
}

    QVariantMap limits;
    limits.insert("start", start);
    limits.insert("end", end);
    params.insert("limits", limits);
    koDebug(XDAREA_LIBRARY) << "requesting items. From:" << start << "to:" << end;

    if (m_albumId == KodiModel::ItemIdRecentlyAdded && m_artistId == KodiModel::ItemIdRecentlyAdded) {
        KodiConnection::sendCommand("AudioLibrary.GetRecentlyAddedSongs", params, this, "listReceived");
    } else if (m_albumId == KodiModel::ItemIdRecentlyPlayed && m_artistId == KodiModel::ItemIdRecentlyPlayed) {
        KodiConnection::sendCommand("AudioLibrary.GetRecentlyPlayedSongs", params, this, "listReceived");
    } else {
        KodiConnection::sendCommand("AudioLibrary.GetSongs", params, this, "listReceived");
    }
}

void Songs::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("songid", m_list.at(index)->data(RoleSongId).toInt());

    QVariantList properties;
//    properties.append("title");
//    properties.append("artist");
//    properties.append("albumartist");
    properties.append("genre");
    properties.append("year");
    properties.append("rating");
//    properties.append("album");
//    properties.append("track");
    properties.append("duration");
    properties.append("comment");
//    properties.append("lyrics");
//    properties.append("musicbrainztrackid");
//    properties.append("musicbrainzartistid");
//    properties.append("musicbrainzalbumid");
//    properties.append("musicbrainzalbumartistid");
    properties.append("playcount");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("artistid");
//    properties.append("albumid");

    params.insert("properties", properties);

    int id = KodiConnection::sendCommand("AudioLibrary.GetSongDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Songs::download(int index, const QString &path)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QString destination = path + "/Music/" + item->artist() + '/' + item->album() + '/';
    qDebug() << "should download" << destination;

    KodiDownload *download = new KodiDownload();
    download->setDestination(destination);
    download->setIconId("icon-m-content-audio");
    download->setLabel(item->title());

    startDownload(index, download);
}

void Songs::listReceived(const QVariantMap &rsp)
{
//  int startItem = rsp.value("result").toMap().value("limits").toMap().value("start").toInt();
    int endItem = rsp.value("result").toMap().value("limits").toMap().value("end").toInt();
    int totalItems = rsp.value("result").toMap().value("limits").toMap().value("total").toInt();

    QList<KodiModelItem*> list;
//    qDebug() << "got songs:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("songs").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        QString subTitle = itemMap.value("artist").toString();
        if (!itemMap.value("artist").toString().isEmpty() && !itemMap.value("album").toString().isEmpty()) {
            subTitle += " - ";
        }
        subTitle += itemMap.value("album").toString();
        item->setSubtitle(subTitle);
        item->setArtist(itemMap.value("artist").toString());
        item->setAlbum(itemMap.value("album").toString());
        item->setSongId(itemMap.value("songid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setFileName(itemMap.value("file").toString());
        item->setIgnoreArticle(false); // Ignoring article here...
        item->setFileType("file");
        item->setPlayable(true);
        item->setYear(itemMap.value("year").toString());
        list.append(item);
    }
    koDebug(XDAREA_LIBRARY) << "inserting items. FromIndex:"<< m_list.count() << "toIndex:" << m_list.count() + list.count() - 1 << "Total:" << totalItems;
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count() + list.count() - 1);
    foreach(KodiModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();

    if (endItem < totalItems) {
        refresh(endItem, qMin(endItem + 200, totalItems));
    } else {
        setBusy(false);
    }
}

void Songs::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("songdetails").toMap();
    item->setYear(details.value("year").toString());
    item->setRating(details.value("rating").toInt());
    item->setDuration(QTime().addSecs(details.value("duration").toInt()));
    item->setComment(details.value("comment").toString());
    item->setPlaycount(details.value("playcount").toInt());
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

KodiModel* Songs::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter song items. Use playItem() to play it";
    return 0;
}

void Songs::playItem(int row, bool resume)
{
    AudioPlaylistItem pItem;
    if(m_artistId < 0 && m_albumId < 0) {
        pItem.setSongId(index(row, 0, QModelIndex()).data(RoleSongId).toInt());
    } else if(m_albumId < 0){
        pItem.setArtistId(m_artistId);
    } else {
        pItem.setAlbumId(m_albumId);
    }
    Kodi::instance()->audioPlayer()->open(pItem, row, resume);
}

void Songs::addToPlaylist(int row)
{
    AudioPlaylistItem pItem;
    pItem.setSongId(index(row, 0, QModelIndex()).data(RoleSongId).toInt());
    Kodi::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Songs::title() const
{
    return tr("Songs");
}
