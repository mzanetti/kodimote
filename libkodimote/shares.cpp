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

#include "shares.h"
#include "files.h"
#include "kodiconnection.h"
#include "kodebug.h"
#include "libraryitem.h"

Shares::Shares(const QString &mediatype, KodiLibrary *parent):
    KodiLibrary(parent),
    m_mediaType(mediatype)
{
    if (m_mediaType.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, 1);

        LibraryItem *item = new LibraryItem(this);
        item->setTitle(tr("Music"));
        item->setFileName("music");
        item->setFileType("directory");
        m_list.append(item);

        item = new LibraryItem(this);
        item->setTitle(tr("Videos"));
        item->setFileName("video");
        item->setFileType("directory");
        m_list.append(item);

        endInsertRows();
        setBusy(false);
    }
}

void Shares::refresh()
{
    if (m_mediaType.isEmpty()) {
        return;
    }
    QVariantMap params;

//    QVariant media(mediaString());
    params.insert("media", m_mediaType);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", ignoreArticle());
    params.insert("sort", sort);

    koDebug(XDAREA_FILES) << "Files.GetShares:" << params;
    KodiConnection::sendCommand("Files.GetSources", params, this, "sourcesReceived");
}

void Shares::sourcesReceived(const QVariantMap &rsp)
{
    koDebug(XDAREA_FILES) << "Files reponse:" << rsp;
    setBusy(false);
    QList<KodiModelItem*> list;
    koDebug(XDAREA_FILES) << "got shares:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("sources").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        item->setFileName(itemMap.value("file").toString());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("directory");
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(KodiModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

KodiModel *Shares::enterItem(int index)
{
    if (m_mediaType.isEmpty()) {
        return new Shares(m_list.at(index)->data(RoleFileName).toString(), this);
    }
    return new Files(m_mediaType, m_list.at(index)->data(RoleFileName).toString(), this);
}

void Shares::playItem(int index)
{
    Q_UNUSED(index)
    koDebug(XDAREA_FILES) << "Playing whole shares is not supported";
}

void Shares::addToPlaylist(int index)
{
    Q_UNUSED(index)
}

QString Shares::title() const
{
    if(m_mediaType == "music") {
        return tr("Music Files");
    } else if(m_mediaType == "video"){
        return tr("Video Files");
    } else if(m_mediaType == "pictures") {
        return tr("Picture Files");
    }
    return tr("Shares");
}

bool Shares::allowSearch()
{
    return false;
}
