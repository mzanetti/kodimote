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
        item->setData(itemMap.value("movieid").toInt(), RoleMovieId);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Movies::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Movies::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case RoleFileType:
        return "file";
    case RoleSubtitle:
        return "";
    case RolePlayable:
        return true;
    }
    return QVariant();
}

XbmcModel *Movies::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter Movie. Use playItem() to play it";
    return 0;
}

void Movies::playItem(int index)
{
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item(m_list.at(index)->data(RoleMovieId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playlist()->playItem(0);
}

void Movies::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setMovieId(m_list.at(row)->data(RoleMovieId).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString Movies::title() const
{
    return "Movies";
}
