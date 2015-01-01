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

#include "channelgroups.h"
#include "channels.h"

#include "kodiconnection.h"
#include "libraryitem.h"

ChannelGroups::ChannelGroups(KodiModel *parent) :
    KodiLibrary(parent)
{
}

QString ChannelGroups::title() const
{
    return tr("TV Channels");
}

void ChannelGroups::refresh()
{
    QVariantMap params;
    params.insert("channeltype", "tv");

    KodiConnection::sendCommand("PVR.GetChannelGroups", params, this, "listReceived");

}

KodiModel *ChannelGroups::enterItem(int index)
{
    qDebug() << "entering" << m_list.at(index)->data(RoleChannelGroupId).toInt();
    return new Channels(m_list.at(index)->data(RoleChannelGroupId).toInt(), this);
}

void ChannelGroups::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "cannot play channelgroup";
}

void ChannelGroups::addToPlaylist(int index)
{
    Q_UNUSED(index);
}

void ChannelGroups::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got channelgroups:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("channelgroups").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        item->setFileType("directory");
        //itemMap.value("channeltype").toString());
        item->setChannelgroupId(itemMap.value("channelgroupid").toInt());
        item->setIgnoreArticle(ignoreArticle());
        item->setPlayable(false);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(KodiModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}
