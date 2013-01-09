#include "channels.h"

#include "xbmcconnection.h"

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

    XbmcConnection::sendCommand("Files.GetDirectory", params, this, "listReceived");
}

XbmcModel *Channels::enterItem(int index)
{
    return 0;
}

void Channels::playItem(int index)
{
}

void Channels::addToPlaylist(int index)
{

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
        item->setFileType(itemMap.value("channeltype").toString());
        item->setChannelId(itemMap.value("channelgroupid").toInt());
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
