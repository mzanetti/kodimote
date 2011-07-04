#include "files.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "audioplayer.h"
#include "videoplayer.h"
#include "playlist.h"

Files::Files(const QString &mediaType, const QString &dir, XbmcModel *parent):
    XbmcModel(parent),
    m_mediaType(mediaType),
    m_dir(dir)
{    
    QVariantMap params;
    params.insert("directory", dir);
    params.insert("media", mediaType);

    QVariantList fields;
    fields.append("file");
    params.insert("fields", fields);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_requestId = XbmcConnection::sendCommand("Files.GetDirectory", params);

}

void Files::responseReceived(int id, const QVariantMap &rsp)
{
//    qDebug() << "Files reponse:" << response;
    if(id != m_requestId) {
        return;
    }
    QList<QStandardItem*> list;
    qDebug() << "got files:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("files").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData(itemMap.value("filetype").toString(), Qt::UserRole+1);
        item->setData(itemMap.value("file").toString(), Qt::UserRole + 100);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    foreach(QStandardItem *item, list) {
        m_list.append(item);
    }
    endInsertRows();
}

int Files::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Files::data(const QModelIndex &index, int role) const
{
    return m_list.at(index.row())->data(role);
}

XbmcModel *Files::enterItem(int index)
{
    if(m_list.at(index)->data(Qt::UserRole+1).toString() == "directory") {
        return new Files(m_mediaType, m_list.at(index)->data(Qt::UserRole+100).toString(), this);
    }
    qDebug() << "cannot enter item of type file";
    return 0;
}

void Files::playItem(int index)
{
    Player *player;
    if(m_mediaType == "music") {
        player = Xbmc::instance()->audioPlayer();
        player->playlist()->clear();
        int newListCount = 0;
        int songToPlay = 0;
        for(int i = 0; i < m_list.count(); ++i) {
            QStandardItem *item = m_list.at(i);
            if(item->data(Qt::UserRole+1).toString() == "file") { // only add files (no dirs)
                player->playlist()->addFile(item->data(Qt::UserRole+100).toString());
                if(i == index) {
                    songToPlay = newListCount;
                }
                newListCount++;
            }
        }
        player->playlist()->playItem(songToPlay);
    } else {
        player = Xbmc::instance()->videoPlayer();
        player->playlist()->clear();
        player->playlist()->addFile(m_list.at(index)->data(Qt::UserRole+100).toString());
        player->playlist()->playItem(0);
    }
}

QString Files::title() const
{
    return m_dir;
}
