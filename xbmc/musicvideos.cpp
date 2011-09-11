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

#include "musicvideos.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

MusicVideos::MusicVideos(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetMusicVideos");
}

void MusicVideos::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got MusicCideos:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("musicvideos").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("musicvideoid").toInt(), RoleMusicVideoId);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int MusicVideos::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant MusicVideos::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case RoleFileType:
        return "file";
    case RoleSubtitle:
        return "";
    case RolePlayable:
        return true;
    }
    return QVariant();
}

XbmcModel *MusicVideos::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter MusicVideo. Use playItem() to play it";
    return 0;
}

void MusicVideos::playItem(int index)
{
    qDebug() << "should play item" << index << "musicvideoid is" << m_list.at(index)->data(RoleMusicVideoId).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item;
    item.setMusicVideoId(m_list.at(index)->data(RoleMusicVideoId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playlist()->playItem(0);
}

void MusicVideos::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setMusicVideoId(m_list.at(row)->data(RoleMusicVideoId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString MusicVideos::title() const
{
    return "Music Videos";
}
