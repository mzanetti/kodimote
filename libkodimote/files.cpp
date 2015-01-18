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

#include "files.h"
#include "kodiconnection.h"
#include "kodi.h"
#include "audioplayer.h"
#include "videoplayer.h"
#include "pictureplayer.h"
#include "playlist.h"
#include "audioplaylistitem.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"
#include "kodidownload.h"

Files::Files(const QString &mediaType, const QString &dir, KodiModel *parent):
    KodiLibrary(parent),
    m_mediaType(mediaType),
    m_dir(dir),
    m_sort(true)
{
    if (mediaType == "music") {
        m_player = Kodi::instance()->audioPlayer();
    } else if (mediaType == "video") {
        m_player = Kodi::instance()->videoPlayer();
    } else {
        m_player = Kodi::instance()->picturePlayer();
    }
}

void Files::refresh()
{
    QVariantMap params;
    params.insert("directory", m_dir);
    params.insert("media", m_mediaType);

    QVariantList properties;
    properties.append("file");
    if(m_mediaType != "pictures") {
        properties.append("thumbnail");
    }
    params.insert("properties", properties);

    if (m_sort) {
        QVariantMap sort;
        sort.insert("method", "label");
        sort.insert("order", "ascending");
        sort.insert("ignorearticle", ignoreArticle());
        params.insert("sort", sort);
    }

    KodiConnection::sendCommand("Files.GetDirectory", params, this, "listReceived");
}

void Files::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got files:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("files").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        if (!filterFile(itemMap)) {
            continue;
        }

        LibraryItem *item = new LibraryItem(this);
        item->setTitle(parseTitle(itemMap.value("label").toString()));
        item->setFileType(itemMap.value("filetype").toString());
        item->setFileName(itemMap.value("file").toString());
        if(m_mediaType == "pictures") {
            item->setThumbnail(itemMap.value("file").toString());
        } else {
            item->setThumbnail(itemMap.value("thumbnail").toString());
        }
        item->setIgnoreArticle(ignoreArticle());
        item->setPlayable(true);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

KodiModel *Files::enterItem(int index)
{
    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    if(item->fileType() == "directory") {
        return new Files(m_mediaType, item->fileName(), this);
    }
    qDebug() << "cannot enter item of type file";
    return 0;
}

void Files::playItem(int index)
{
    if (m_mediaType == "picture") {
        m_player->stop();
        QTimer::singleShot(100, Kodi::instance(), SLOT(queryActivePlayers()));
    }

    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    PlaylistItem pItem;
    m_player->playlist()->clear();
    if(item->fileType() == "file") {
        pItem.setFile(item->fileName());
    } else {
        pItem.setDirectory(item->fileName());
    }
    m_player->playlist()->addItems(pItem);
    m_player->playItem(0);
}

void Files::addToPlaylist(int index)
{
    LibraryItem *item = static_cast<LibraryItem*>(m_list.at(index));
    PlaylistItem pItem;
    if(item->fileType() == "file") {
        pItem.setFile(item->fileName());
    } else {
        pItem.setDirectory(item->fileName());
    }
    m_player->playlist()->addItems(pItem);
}

QString Files::title() const
{
    return m_dir;
}

void Files::download(int index, const QString &path)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QString destination;
    if(m_mediaType == "music") {
        destination = path + "/Music/" + item->tvShow() + "/Season " + item->season() + '/';
    } else if(m_mediaType == "video") {
        destination = path + "/Movies/" + item->tvShow() + "/Season " + item->season() + '/';
    } else if(m_mediaType == "pictures") {
        destination = path + "//" + item->tvShow() + "/Season " + item->season() + '/';
    }

    qDebug() << "should download" << destination;

    KodiDownload *download = new KodiDownload();
    download->setDestination(destination);
    download->setIconId("icon-m-content-videos");
    download->setLabel(item->title());

    startDownload(index, download);
}

QString Files::parseTitle(const QString &title) const
{
    return title;
}

bool Files::filterFile(const QVariantMap &) const
{
    return true;
}
