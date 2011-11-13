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

#include "songs.h"
#include "audioplayer.h"
#include "audioplaylist.h"
#include "audioplaylistitem.h"
#include "xbmc.h"
#include "xbmcconnection.h"

Songs::Songs(int artistid, int albumid, XbmcModel *parent):
    XbmcLibrary(parent),
    m_artistId(artistid),
    m_albumId(albumid)
{
    QVariantMap params;
    if(artistid != -1) {
      params.insert("artistid", artistid);
    }
    if(albumid != -1) {
      params.insert("albumid", albumid);
    }
    QVariantList properties;
    properties.append("artist");
    properties.append("album");
    params.insert("properties", properties);

    QVariantMap sort;
    if(m_albumId == -1) {
        sort.insert("method", "label");
    } else {
        sort.insert("method", "track");
    }
    sort.insert("order", "ascending");
    params.insert("sort", sort);
    m_requestId = XbmcConnection::sendCommand("AudioLibrary.GetSongs", params);
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
}

void Songs::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_requestId) {
        return;
    }
    QList<QStandardItem*> list;
//    qDebug() << "got songs:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("songs").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("artist").toString() + " - " + itemMap.value("album").toString(), RoleSubtitle);
        item->setData(itemMap.value("songid").toInt(), RoleSongId);
        item->setData(itemMap.value("label").toString(), RoleSortingTitle);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(QStandardItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();

}

int Songs::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Songs::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleFileType:
        return "file";
    case RolePlayable:
        return true;
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel* Songs::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter song items. Use playItem() to play it";
    return 0;
}

void Songs::playItem(int row)
{
    AudioPlaylistItem pItem;
    if(m_artistId == -1 && m_albumId == -1) {
        pItem.setSongId(index(row, 0, QModelIndex()).data(RoleSongId).toInt());
    } else if(m_albumId == -1){
        pItem.setArtistId(m_artistId);
    } else {
        pItem.setAlbumId(m_albumId);
    }
    Xbmc::instance()->audioPlayer()->playlist()->clear();
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
//    Xbmc::instance()->audioPlayer()->playlist()->playItem(row);
    Xbmc::instance()->audioPlayer()->playItem(row);
}

void Songs::addToPlaylist(int row)
{
    AudioPlaylistItem pItem;
    pItem.setSongId(index(row, 0, QModelIndex()).data(RoleSongId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Songs::title() const
{
    return "Songs";
}
