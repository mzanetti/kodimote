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

#include "audiolibrary.h"
#include "player.h"
#include "xbmcconnection.h"
#include "artistitem.h"
#include "playlist.h"

AudioLibrary::AudioLibrary(Player *player, QObject *parent) :
    QAbstractItemModel(parent),
    m_player(player)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    m_state = "library";

    // intialize with default stuff
    m_list.append(LibraryItem("Artists", 0));
    m_list.append(LibraryItem("Albums", 1));
    m_list.append(LibraryItem("Songs", 2));

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "itemId");
    setRoleNames(roleNames);

    m_albumFilter = -1;
    m_artistFilter = -1;

}

void AudioLibrary::enterItem(int index)
{
    qDebug() << "entering item" << index << "state is" << m_state;
    if(m_state == "library") {
        switch(index) {
        case 0:
            showArtists();
            break;
        case 1:
            showAlbums();
            break;
        case 2:
            showSongs();
            break;
        }
    }

    if(m_state == "artists") {
        showAlbums(index);
        return;
    }

    if(m_state == "albums") {
        showSongs(m_artistFilter, index);
    }

    if(m_state == "songs") {
        qDebug() << "on song pressed" << index;
        m_player->playlist()->clear();
        m_player->playlist()->addItems(m_artistFilter, m_albumFilter);
        for(int i = 0; i < m_songList.count(); ++i) {
            if(m_songList.at(i).songId() == index) {
                m_player->playlist()->playItem(i);
                break;
            }
        }
    }
}

void AudioLibrary::showLibrary()
{
    beginResetModel();
    m_state = "library";
    emit stateChanged();
    endResetModel();
}

void AudioLibrary::showArtists()
{
    qDebug() << "requesting artists";
    QVariantMap params;
    params.insert("genreid", 1);
    int id = XbmcConnection::sendCommand("AudioLibrary.GetArtists");//, params);
    m_requestMap.insert(id, RequestArtists);
}

void AudioLibrary::showAlbums(int artistId)
{
    qDebug() << "requesting albums with artistid" << artistId;
    m_artistFilter = artistId;
    QVariantMap params;
//    params.insert("genreid", -1);
    if(artistId != -1) {
      params.insert("artistid", artistId);
    }
//    params.insert("start", 0);
//    params.insert("end", 50);
    int id = XbmcConnection::sendCommand("AudioLibrary.GetAlbums", params);
    m_requestMap.insert(id, RequestAlbums);
}

void AudioLibrary::showSongs(int artistId, int albumId)
{
    qDebug() << "requesting songs with artistid" << artistId << "and albumid" << albumId;
    m_artistFilter = artistId;
    m_albumFilter = albumId;
    QVariantMap params;
//    params.insert("genreid", -1);
    if(artistId != -1) {
        params.insert("artistid", artistId);
    }
    if(albumId != -1) {
        params.insert("albumid", albumId);
    }
    QVariantList fields;
    fields.append("file");
    params.insert("fields", fields);
    int id = XbmcConnection::sendCommand("AudioLibrary.GetSongs", params);
    m_requestMap.insert(id, RequestSongs);
}

QModelIndex AudioLibrary::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex AudioLibrary::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int AudioLibrary::rowCount(const QModelIndex &parent) const
{
    if(m_state == "library") {
        return m_list.count();
    }
    if(m_state == "artists") {
        return m_artistList.count();
    }
    if(m_state == "albums") {
        return m_albumList.count();
    }
    if(m_state == "songs") {
        return m_songList.count();
    }
    return 0;
}

int AudioLibrary::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant AudioLibrary::data(const QModelIndex &index, int role) const
{
    if(m_state == "library") {
        if(role == Qt::DisplayRole) {
            return m_list.at(index.row()).label();
        }
        return m_list.at(index.row()).id();
    } else if(m_state == "artists") {
        if(role == Qt::DisplayRole) {
            return m_artistList.at(index.row()).label();
        }
        return m_artistList.at(index.row()).id();
    } else if(m_state == "albums") {
        if(role == Qt::DisplayRole) {
            return m_albumList.at(index.row()).label();
        }
        return m_albumList.at(index.row()).id();
    } else if(m_state == "songs") {
        if(role == Qt::DisplayRole) {
            return m_songList.at(index.row()).label();
        }
        return m_songList.at(index.row()).songId();
    }
    return QVariant();
}

void AudioLibrary::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestArtists: {
        beginResetModel();
        m_artistList.clear();
        qDebug() << "got artists:" << rsp;
        QVariantList responseList = rsp.toMap().value("artists").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            ArtistItem item(itemMap.value("label").toString(), itemMap.value("artistid").toInt());
            qDebug() << "adding item:" << item.label();
            m_artistList.append(item);
            m_state = "artists";
            emit stateChanged();
        }
        endResetModel();
    }
        break;
    case RequestAlbums: {
        beginResetModel();
        m_albumList.clear();
        qDebug() << "got albums:" << response;
        QVariantList responseList = rsp.toMap().value("albums").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            AlbumItem item(itemMap.value("label").toString(), itemMap.value("albumid").toInt());
            qDebug() << "adding item:" << item.label();
            m_albumList.append(item);
            m_state = "albums";
            emit stateChanged();
        }
        endResetModel();
        break;
    }
    case RequestSongs: {
        beginResetModel();
        m_songList.clear();
//        qDebug() << "got songs:" << response;
        QVariantList responseList = rsp.toMap().value("songs").toList();
        qDebug() << "starting inserting items";
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
//            qDebug() << "got songitem" << itemMap;
            SongItem item;
            item.setSongId(itemMap.value("songid").toInt());
            item.setFile(itemMap.value("file").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setFanart(itemMap.value("fanart").toString());
            item.setThumbnail(itemMap.value("thumbnail").toString());
//            qDebug() << "adding item:" << item.label();
            m_songList.append(item);
            m_state = "songs";
            emit stateChanged();
        }
        endResetModel();
        break;
    }
    }
}


QString AudioLibrary::state()
{
    return m_state;
}

int AudioLibrary::albumFilter()
{
    return m_albumFilter;
}

int AudioLibrary::artistFilter()
{
    return m_artistFilter;
}

QString AudioLibrary::currentDir()
{
    if(m_state == "library") {
        return "Library/";
    }

    if(m_state == "artists") {
        return "Library/Artists/";
    }

    if(m_state == "albums") {
        if(m_artistFilter == -1) {
            return "Library/Albums/";
        } else {
            return "Library/Artists/Albums/";
        }
    }

    if(m_state == "songs") {
        QString ret = "Library/";
        if(m_artistFilter != -1) {
            ret.append("Artists/");
        }
        if(m_albumFilter != -1) {
            ret.append("Albums/");
        }
        ret.append("Songs/");
        return ret;
    }
}

void AudioLibrary::goUp(int levels)
{
    if(levels <= 0 || m_state == "library") {
        return; // Nothing to do
    }

    if(m_state == "artists") { // Library > Artists
        showLibrary();
        return;
    }

    if(m_state == "albums") {
        if(m_artistFilter == -1) { // Library > Albums
            showLibrary();
        } else {  // Library > Artists > Albums
            if(levels == 1) {
                showArtists();
            } else if(levels > 1) {
                showLibrary();
            }
        }
        return;
    }

    if(m_state == "songs") {
        if(m_artistFilter != -1) {
            if(m_albumFilter != -1) { // Library > Artists > Albums > Songs
                if(levels == 1) {
                    showAlbums(m_artistFilter);
                } else if(levels == 2) {
                    showArtists();
                } else {
                    showLibrary();
                }
            } else { // Library > Artists > Songs (not yet supported)
                showArtists();
            }
        } else {
            if(m_albumFilter != -1) { // Library > Albums > Songs
                if(levels == 1) {
                    showAlbums();
                } else {
                    showLibrary();
                }
            } else { // Library > Songs
                showLibrary();
            }
        }
        return;
    }
}
