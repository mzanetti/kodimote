#include "channelgroups.h"
#include "channels.h"

#include "xbmcconnection.h"
#include "libraryitem.h"

ChannelGroups::ChannelGroups(XbmcModel *parent) :
    XbmcLibrary(parent)
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

    XbmcConnection::sendCommand("PVR.GetChannelGroups", params, this, "listReceived");

}

XbmcModel *ChannelGroups::enterItem(int index)
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

}

void ChannelGroups::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<XbmcModelItem*> list;
    qDebug() << "got channelgroups:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("channelgroups").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setFileType("directory");
        //itemMap.value("channeltype").toString());
        item->setChannelgroupId(itemMap.value("channelgroupid").toInt());
        item->setIgnoreArticle(ignoreArticle());
        item->setPlayable(false);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(XbmcModelItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}
