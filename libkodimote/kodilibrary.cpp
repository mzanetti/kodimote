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

#include "kodilibrary.h"
#include "libraryitem.h"
#include "kodihostmodel.h"
#include "kodiconnection.h"
#include "kodidownload.h"
#include "kodi.h"
#include "player.h"
#include "audioplayer.h"
#include "videoplayer.h"

#include "libraryitem.h"

#include <QTimer>
#include <QFileInfo>

KodiLibrary::KodiLibrary(KodiModel *parent) :KodiModel(parent), m_deleteAfterDownload(false)
{
    // Refresh the model automatically on the next event loop run.
    // This is to give QML time to create the object and set properties before the refresh
    QTimer::singleShot(0, this, SLOT(refresh()));

    connect(Kodi::instance()->audioPlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
    connect(Kodi::instance()->videoPlayer(), SIGNAL(currentItemChanged()), SLOT(currentItemChanged()));
    connect(Kodi::instance()->audioPlayer(), SIGNAL(stateChanged()), SLOT(currentItemChanged()));
    connect(Kodi::instance()->videoPlayer(), SIGNAL(stateChanged()), SLOT(currentItemChanged()));
}

KodiLibrary::~KodiLibrary()
{
}

QVariant KodiLibrary::data(const QModelIndex &index, int role) const
{
    if(role == RolePlayingState) {
        if(!Kodi::instance()->activePlayer() || !Kodi::instance()->activePlayer()->currentItem()) {
            return "";
        }
        LibraryItem *thisItem = qobject_cast<LibraryItem*>(m_list.at(index.row()));
        LibraryItem *currentItem = Kodi::instance()->activePlayer()->currentItem();
        if(currentItem->artistId() == -1 &&
                    currentItem->songId() == -1 &&
                    currentItem->albumId() == -1 &&
                    currentItem->movieId() == -1 &&
                    currentItem->episodeId() == -1 &&
                    currentItem->channelId() == -1) {

            if(thisItem->fileName() == currentItem->fileName()) {
                return Kodi::instance()->activePlayer()->state();
            }
            return "";
        }
        if(thisItem->artistId() == currentItem->artistId() &&
               thisItem->albumId() == currentItem->albumId() &&
               thisItem->songId() == currentItem->songId() &&
               thisItem->movieId() == currentItem->movieId() &&
               thisItem->episodeId() == currentItem->episodeId() &&
               thisItem->channelId() == currentItem->channelId()) {
            return Kodi::instance()->activePlayer()->state();
        }
        return "";
    }


    return KodiModel::data(index, role);
}

KodiModel* KodiLibrary::exit()
{
    deleteLater();
    return m_parentModel;
}

QVariant KodiLibrary::get(int row, const QString &roleName)
{
    return data(index(row, 0, QModelIndex()), roleNames().key(roleName.toLatin1()));
}

KodiModelItem *KodiLibrary::getItem(int index)
{
    return m_list.at(index);
}

void KodiLibrary::download(int index, const QString &path)
{
    Q_UNUSED(index);
    Q_UNUSED(path);
    qDebug() << "This model does not support downloading";
}

void KodiLibrary::setDeleteAfterDownload(bool deleteAfterDownload)
{
    m_deleteAfterDownload = deleteAfterDownload;
}

bool KodiLibrary::deleteAfterDownload() const
{
    return m_deleteAfterDownload;
}

void KodiLibrary::startDownload(int index, KodiDownload *download)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QFileInfo fileInfo(item->fileName().replace('\\', '/')); // Make sure it works on Windoze too
    download->setDestination(download->destination() + fileInfo.fileName());

    QVariantMap params;
    params.insert("path", item->fileName());
    int id = KodiConnection::sendCommand("Files.PrepareDownload", params, this, "downloadReceived");
    m_downloadMap.insert(id, download);
}

void KodiLibrary::downloadReceived(const QVariantMap &rsp)
{
    int id = rsp.value("id").toInt();
    KodiDownload *download = m_downloadMap.take(id);
    if(rsp.contains("error")) {
        delete download;
        return;
    }
    QString path = rsp.value("result").toMap().value("details").toMap().value("path").toString();
    qDebug() << "Downloading" << path;

    download->setSource(path);

    KodiConnection::download(download);

    if(m_deleteAfterDownload && m_downloadMap.isEmpty()) {
        qDebug() << "Deleting download model";
        deleteLater();
    }
}

void KodiLibrary::currentItemChanged()
{
    emit dataChanged(index(0, 0, QModelIndex()), index(m_list.count()-1, 0, QModelIndex()));
}
