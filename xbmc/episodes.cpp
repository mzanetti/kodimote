#include "episodes.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

Episodes::Episodes(int tvshowid, int seasonid, const QString &seasonString, XbmcModel *parent):
    XbmcModel(parent),
    m_seasonString(seasonString)
{
    QVariantMap params;
    if(tvshowid != -1) {
      params.insert("tvshowid", tvshowid);
    }
    if(tvshowid != -1) {
      params.insert("season", seasonid);
    }
    QVariantList fields;
    fields.append("showtitle");
//    fields.append("season");
    params.insert("fields", fields);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetEpisodes", params);
}

void Episodes::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got Episodes:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("episodes").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
//        item->setData(itemMap.value("showtitle").toString() + " - " + itemMap.value("season").toString(), Qt::UserRole+2);
        item->setData(itemMap.value("showtitle").toString() + " - " + m_seasonString, Qt::UserRole+2);
        item->setData(itemMap.value("episodeid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Episodes::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Episodes::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::UserRole+1:
        return "file";
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Episodes::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter Episode. Use playItem() to play it";
    return 0;
}

void Episodes::playItem(int index)
{
    qDebug() << "should play item" << index << "episodeid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item;
    item.setEpisodeId(m_list.at(index)->data(Qt::UserRole+100).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playlist()->playItem(0);
}

QString Episodes::title() const
{
    return "Episodes";
}
