#include "movies.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

Movies::Movies(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetMovies");
}

Movies::~Movies()
{
}

void Movies::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got movies:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("movies").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("movieid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Movies::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Movies::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case Qt::UserRole+1:
        return "file";
    case Qt::UserRole+2:
        return "";
    }
    return QVariant();
}

XbmcModel *Movies::enterItem(int index)
{
//    return new Albums(m_list.at(index)->data(Qt::UserRole + 100).toInt());
    return 0;
}

void Movies::playItem(int index)
{
    qDebug() << "should play item" << index << "movieid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    qDebug() << "should play item" << index << "movieid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    VideoPlaylistItem item(m_list.at(index)->data(Qt::UserRole+100).toInt());
    qDebug() << "should play item" << index << "movieid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    qDebug() << "should play item" << index << "movieid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->playItem(0);
    qDebug() << "should play item" << index << "movieid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
}

QString Movies::title() const
{
    return "Movies";
}
