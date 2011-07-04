#include "tvshows.h"
#include "seasons.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

TvShows::TvShows(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetTVShows");
}

TvShows::~TvShows()
{
}

void TvShows::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got TvShows:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("tvshows").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("tvshowid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int TvShows::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant TvShows::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case Qt::UserRole+1:
        return "directory";
    case Qt::UserRole+2:
        return "";
    }
    return QVariant();
}

XbmcModel *TvShows::enterItem(int index)
{
    return new Seasons(m_list.at(index)->data(Qt::UserRole + 100).toInt(), this);
}

void TvShows::playItem(int index)
{
    qDebug() << "TvShows: playing whole tvshow not supported yet";
}

QString TvShows::title() const
{
    return "TV Shows";
}
