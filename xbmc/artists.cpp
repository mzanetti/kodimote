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


    m_request = XbmcConnection::sendCommand("AudioLibrary.GetArtists", params);
}

Artists::~Artists()
{
}

void Artists::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got artists:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("artists").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData("directory", Qt::UserRole);
        item->setData(itemMap.value("artistid").toInt(), RoleArtistId);
        item->setData(itemMap.value("thumbnail").toString(), RoleThumbnail);
        item->setData(ignoreArticle(itemMap.value("label").toString()), RoleSortingTitle);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
    emit layoutChanged();
}

int Artists::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Artists::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleFileType:
        return "directory";
    case RoleSubtitle:
        return "";
    case RolePlayable:
        return true;
    }
    return m_list.at(index.row())->data(role);
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
