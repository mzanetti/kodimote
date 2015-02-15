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

#include "genres.h"
#include "kodiconnection.h"
#include "kodi.h"
#include "artists.h"
#include "audioplaylistitem.h"
#include "audioplayer.h"
#include "playlist.h"
#include "libraryitem.h"

Genres::Genres(KodiModel *parent) :
    KodiLibrary(parent)
{
}

void Genres::refresh()
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("ignorearticle", ignoreArticle());
    sort.insert("method", "label");
    params.insert("sort", sort);

    QVariantList properties;
    properties.append("thumbnail");
    params.insert("properties", properties);

    KodiConnection::sendCommand("AudioLibrary.GetGenres", params, this, "listReceived");
}

Genres::~Genres()
{
}

void Genres::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got genres:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("genres").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        item->setFileName("directory");
        item->setGenreId(itemMap.value("genreid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("directory");
        item->setPlayable(true);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
    emit layoutChanged();
}

KodiModel *Genres::enterItem(int index)
{
    return new Artists(m_list.at(index)->data(RoleGenreId).toInt(), this);
}

void Genres::playItem(int index, bool resume)
{
    Q_UNUSED(index)
    Q_UNUSED(resume)
}

void Genres::addToPlaylist(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_list.at(index)->data(RoleArtistId).toInt());
    Kodi::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Genres::title() const
{
    return tr("Genres");
}
