#include "musicvideos.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"

MusicVideos::MusicVideos(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("VideoLibrary.GetMusicVideos");
}

void MusicVideos::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got MusicCideos:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("musicvideos").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("musicvideoid").toInt(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int MusicVideos::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant MusicVideos::data(const QModelIndex &index, int role) const
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

XbmcModel *MusicVideos::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter MusicVideo. Use playItem() to play it";
    return 0;
}

void MusicVideos::playItem(int index)
{
    qDebug() << "should play item" << index << "musicvideoid is" << m_list.at(index)->data(Qt::UserRole+100).toInt();
    Xbmc::instance()->videoPlayer()->playlist()->clear();
    VideoPlaylistItem item;
    item.setMusicVideoId(m_list.at(index)->data(Qt::UserRole+100).toInt());
    Xbmc::instance()->videoPlayer()->playlist()->addItems(item);
    Xbmc::instance()->videoPlayer()->playlist()->playItem(0);
}

QString MusicVideos::title() const
{
    return "Music Videos";
}
