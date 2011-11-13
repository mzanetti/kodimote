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

#include "shares.h"
#include "files.h"
#include "xbmcconnection.h"
#include "xdebug.h"

Shares::Shares(const QString &mediatype):
    XbmcLibrary(0),
    m_mediaType(mediatype)
{
    QVariantMap params;

//    QVariant media(mediaString());
    params.insert("media", mediatype);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    xDebug(XDAREA_FILES) << "Files.GetShares:" << params;
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_requestId = XbmcConnection::sendCommand("Files.GetSources", params);
}

void Shares::responseReceived(int id, const QVariantMap &rsp)
{

    xDebug(XDAREA_FILES) << "Files reponse:" << rsp;
    if(id != m_requestId) {
        return;
    }
    QList<XbmcModelItem*> list;
    xDebug(XDAREA_FILES) << "got shares:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("sources").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setFileName(itemMap.value("file").toString());
        item->setIgnoreArticle(true);
        item->setFileType("directory");
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(XbmcModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

XbmcModel *Shares::enterItem(int index)
{
    return new Files(m_mediaType, m_list.at(index)->data(RoleFileName).toString(), this);
}

void Shares::playItem(int index)
{
    Q_UNUSED(index)
    xDebug(XDAREA_FILES) << "Playing whole shares is not supported";
}

void Shares::addToPlaylist(int index)
{
    Q_UNUSED(index)
}

QString Shares::title() const
{
    if(m_mediaType == "music") {
        return "Music Files";
    } else if(m_mediaType == "video"){
        return "Video Files";
    } else if(m_mediaType == "pictures") {
        return "Picture Files";
    }
    return "";
}
