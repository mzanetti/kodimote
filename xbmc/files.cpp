#include "files.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "audioplayer.h"
#include "videoplayer.h"
#include "playlist.h"
#include "audioplaylistitem.h"
#include "videoplaylistitem.h"

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
        item->setData(itemMap.value("filetype").toString(), RoleFileType);
        item->setData(itemMap.value("file").toString(), RoleFileName);
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
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Files::data(const QModelIndex &index, int role) const
{
    if(role == RolePlayable) {
        return index.data(RoleFileType).toString() == "file";
    }
    return m_list.at(index.row())->data(role);
}

XbmcModel *Files::enterItem(int index)
{
    if(m_list.at(index)->data(RoleFileType).toString() == "directory") {
        return new Files(m_mediaType, m_list.at(index)->data(RoleFileName).toString(), this);
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
            if(item->data(RoleFileType).toString() == "file") { // only add files (no dirs)
                player->playlist()->addFile(item->data(RoleFileName).toString());
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
        player->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
        player->playlist()->playItem(0);
    }
}

void Files::addToPlaylist(int index)
{
    if(m_mediaType == "music") {
        Xbmc::instance()->audioPlayer()->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
    } else {
        Xbmc::instance()->videoPlayer()->playlist()->addFile(m_list.at(index)->data(RoleFileName).toString());
    }
}

QString Files::title() const
{
    return m_dir;
}
