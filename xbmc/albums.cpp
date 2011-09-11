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

Albums::Albums(int artistId, XbmcModel *parent) :
    XbmcModel(parent),
    m_artistId(artistId)
{
    QVariantMap params;
    if(artistId != -1) {
      params.insert("artistid", artistId);
    }
    QVariantList fields;
    fields.append("artist");
    params.insert("fields", fields);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);
    m_request = XbmcConnection::sendCommand("AudioLibrary.GetAlbums", params);
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
}

Albums::~Albums()
{
}

void Albums::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got albums:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("albums").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("artist").toString(), RoleSubtitle);
        item->setData(itemMap.value("albumid").toInt(), RoleAlbumId);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Albums::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Albums::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleFileType:
        return "directory";
    case RolePlayable:
        return true;
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel* Albums::enterItem(int index)
{
    return new Songs(m_artistId, m_list.at(index)->data(RoleAlbumId).toInt(), this);
}

void Albums::playItem(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_artistId);
    pItem.setAlbumId(m_list.at(index)->data(RoleAlbumId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->clear();
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
    Xbmc::instance()->audioPlayer()->playlist()->playItem(0);
}

void Albums::addToPlaylist(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_artistId);
    pItem.setAlbumId(m_list.at(index)->data(RoleAlbumId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Albums::title() const
{
    return "Albums";
}
