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

#include "audioplaylist.h"

#include "xbmcconnection.h"

AudioPlaylist::AudioPlaylist(Player *parent):
    Playlist(parent)
{
}

QString AudioPlaylist::namespaceString() const
{
    return "AudioPlaylist";
}

void AudioPlaylist::refresh()
{
    QVariantMap params;
    QVariantList fields;
    fields.append("duration");
    params.insert("fields", fields);

    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems", params);
    m_requestMap.insert(id, RequestGetItems);
}


void AudioPlaylist::queryItemData(int index)
{
    QVariantMap params;
    QVariantList fields;
    fields.append("title");
    fields.append("artist");
    fields.append("album");
    fields.append("fanart");
    fields.append("thumbnail");
    fields.append("duration");
    params.insert("fields", fields);

    QVariantMap limits;
    limits.insert("start", index);
    limits.insert("end", index + 1);
    params.insert("limits", limits);

    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems", params);
    m_requestMap.insert(id, RequestCurrentData);
}


void AudioPlaylist::responseReveiced(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

//    qDebug() << "AudioPlaylist response:" << rsp;

    switch(m_requestMap.value(id)) {
    case RequestGetItems: {
//        qDebug() << "got GetItems response:" << response;
//        qDebug() << "resetting model";
        beginResetModel();
        m_itemList.clear();
        QVariantList responseList = rsp.toMap().value("items").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            SongItem item;
//            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setDuration(QTime().addSecs(itemMap.value("duration").toInt()));
//            item.setTitle(itemMap.value("title").toString());
//            item.setArtist(itemMap.value("artist").toString());
//            qDebug() << "adding item:" << item.label();
            m_itemList.append(item);
        }
        endResetModel();
        m_currentSong = rsp.toMap().value("state").toMap().value("current").toInt();
        qDebug() << "set current to" << m_currentSong;
        queryItemData(m_currentSong);
        emit countChanged();
        emit currentChanged();
        break;
    }
    case RequestCurrentData: {
        if(m_itemList.count() > m_currentSong && m_currentSong > -1) {
            SongItem item = m_itemList.at(m_currentSong);
            QVariantList responseList = rsp.toMap().value("items").toList();
            QVariantMap itemMap = responseList.first().toMap();
    //            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setTitle(itemMap.value("title").toString());
            item.setArtist(itemMap.value("artist").toString());
            item.setAlbum(itemMap.value("album").toString());
            item.setFanart(itemMap.value("fanart").toString());
            item.setThumbnail(itemMap.value("thumbnail").toString());
            m_itemList.replace(m_currentSong, item);
            emit currentChanged();
            break;
        }
        }
    }
}
