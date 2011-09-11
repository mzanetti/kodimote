#include "artists.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "albums.h"
#include "audioplaylistitem.h"
#include "audioplayer.h"
#include "playlist.h"

Artists::Artists(XbmcModel *parent) :
    XbmcModel(parent)
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
    m_request = XbmcConnection::sendCommand("AudioLibrary.GetArtists");
}

Artists::~Artists()
{
}

void Artists::responseReceived(int id, const QVariantMap &rsp)
{
    if(id != m_request) {
        return;
    }

    QList<QStandardItem*> list;
    qDebug() << "got artists:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("artists").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        QStandardItem *item = new QStandardItem();
        item->setText(itemMap.value("label").toString());
        item->setData("directory", Qt::UserRole);
        item->setData(itemMap.value("artistid").toInt(), RoleArtistId);
        list.append(item);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

int Artists::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Artists::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_list.at(index.row())->text();
    case RoleFileType:
        return "directory";
    case RoleSubtitle:
        return "";
    case RolePlayable:
        return true;
    }
    return QVariant();
}

XbmcModel *Artists::enterItem(int index)
{
    return new Albums(m_list.at(index)->data(RoleArtistId).toInt(), this);
}

void Artists::playItem(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_list.at(index)->data(RoleArtistId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->clear();
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
    Xbmc::instance()->audioPlayer()->playlist()->playItem(0);
}

void Artists::addToPlaylist(int index)
{
    AudioPlaylistItem pItem;
    pItem.setArtistId(m_list.at(index)->data(RoleArtistId).toInt());
    Xbmc::instance()->audioPlayer()->playlist()->addItems(pItem);
}

QString Artists::title() const
{
    return "Artists";
}
