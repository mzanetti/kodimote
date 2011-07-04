#include "seasons.h"
#include "episodes.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

Seasons::Seasons(int tvshowid, XbmcModel *parent):
    XbmcModel(parent),
    m_tvshowid(tvshowid)
{
    QVariantMap params;
    if(tvshowid != -1) {
      params.insert("tvshowid", tvshowid);
    }
    QVariantList fields;
    fields.append("showtitle");
    fields.append("season");
    params.insert("fields", fields);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetSeasons", params);
}

void Seasons::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got Seasons:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("seasons").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("showtitle").toString(), Qt::UserRole+2);
        item->setData(itemMap.value("season").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Seasons::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Seasons::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::UserRole+1:
        return "directory";
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Seasons::enterItem(int index)
{
    return new Episodes(m_tvshowid, m_list.at(index)->data(Qt::UserRole + 100).toInt(), m_list.at(index)->text(), this);
}

void Seasons::playItem(int index)
{
    qDebug() << "Seasons: playing whole season not supported yet";
}

QString Seasons::title() const
{
    return "Seasons";
}
