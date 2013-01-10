#include "channels.h"

#include "xbmc.h"
#include "xbmcconnection.h"
#include "libraryitem.h"
#include "videoplaylistitem.h"
#include "videoplayer.h"
#include "playlist.h"

Channels::Channels(int channelgroupid, XbmcModel *parent) :
    m_channelgroupid(channelgroupid),
    XbmcLibrary(parent)
{
}

QString Channels::title() const
{
    return tr("TV Channels");
}

void Channels::refresh()
{
    QVariantMap params;
    params.insert("channelgroupid", m_channelgroupid);

    QVariantList properties;
    properties.append("thumbnail");

    params.insert("properties", properties);

    XbmcConnection::sendCommand("PVR.GetChannels", params, this, "listReceived");
}

XbmcModel *Channels::enterItem(int index)
{
    qDebug() << "cannot enter channel";
    return 0;
}

void Channels::playItem(int index)
{
    VideoPlaylistItem item;
    item.setChannelId(m_list.at(index)->data(RoleChannelId).toInt());
    Xbmc::instance()->videoPlayer()->open(item);
}

void Channels::addToPlaylist(int index)
{

}

void Channels::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("channelid", m_list.at(index)->data(RoleChannelId).toInt());

    QVariantList properties;
    properties.append("thumbnail");
    properties.append("channeltype");
    properties.append("hidden");
    properties.append("locked");
    properties.append("channel");
    properties.append("lastplayed");

    params.insert("properties", properties);

    int id = XbmcConnection::sendCommand("PVR.GetChannelDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Channels::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<XbmcModelItem*> list;
    qDebug() << "got channelgroups:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("channels").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();

        item->setTitle(itemMap.value("label").toString());
        item->setChannelId(itemMap.value("channelid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());

        item->setFileType("file");
        item->setIgnoreArticle(ignoreArticle());
        item->setPlayable(true);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(XbmcModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

void Channels::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("channeldetails").toMap();

    // TODO: fill in details once there is anything interesting

    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}
