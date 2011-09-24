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

#include "files.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "audioplayer.h"
#include "videoplayer.h"
#include "pictureplayer.h"
#include "playlist.h"
#include "audioplaylistitem.h"
#include "videoplaylistitem.h"

Files::Files(const QString &mediaType, const QString &dir, XbmcModel *parent):
    XbmcLibrary(parent),
    m_mediaType(mediaType),
    m_dir(dir)
{    
    QVariantMap params;
    params.insert("directory", dir);
    params.insert("media", mediaType);

    QVariantList properties;
    properties.append("file");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_requestId = XbmcConnection::sendCommand("Files.GetDirectory", params);

}

void Files::responseReceived(int id, const QVariantMap &rsp)
{
//    qDebug() << "Files reponse:" << response;
    if(id != m_requestId) {
        return;
    }
    QList<QStandardItem*> list;
    qDebug() << "got files:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("files").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("filetype").toString(), RoleFileType);
        item->setData(itemMap.value("file").toString(), RoleFileName);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(QStandardItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

int Files::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Files::data(const QModelIndex &index, int role) const
{
    if(role == RolePlayable) {
        return true;
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Files::enterItem(int index)
{
    if(m_list.at(index)->data(RoleFileType).toString() == "directory") {
        return new Files(m_mediaType, m_list.at(index)->data(RoleFileName).toString(), this);
    }
    qDebug() << "cannot enter item of type file";
    return 0;
}

void Files::playItem(int index)
{
    Player *player;
    if(m_mediaType == "music") {
        player = Xbmc::instance()->audioPlayer();
        player->playlist()->clear();
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            player->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            player->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
        player->playItem(0);
    } else if(m_mediaType == "video"){
        player = Xbmc::instance()->videoPlayer();
        player->playlist()->clear();
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            player->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            player->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
        player->playItem(0);
    } else {
        player = Xbmc::instance()->picturePlayer();
        player->stop();
        player->playlist()->clear();
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            player->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            player->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
        player->playItem(0);
        QTimer::singleShot(100, Xbmc::instance(), SLOT(queryActivePlayers()));
    }
}

void Files::addToPlaylist(int index)
{
    if(m_mediaType == "music") {
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            Xbmc::instance()->audioPlayer()->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            Xbmc::instance()->audioPlayer()->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
    } else if(m_mediaType == "video") {
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            Xbmc::instance()->videoPlayer()->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            Xbmc::instance()->videoPlayer()->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
    } else if(m_mediaType == "pictures") {
        if(m_list.at(index)->data(RoleFileType).toString() == "file") {
            Xbmc::instance()->picturePlayer()->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        } else {
            Xbmc::instance()->picturePlayer()->playlist()->addDirectory(m_list.at(index)->data(RoleFileName).toString());
        }
    }
}

QString Files::title() const
{
    return m_dir;
}
