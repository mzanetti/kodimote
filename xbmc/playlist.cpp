#include "playlist.h"

#include "playlistitem.h"
#include "xbmcconnection.h"

namespace Xbmc
{

Playlist::Playlist(QObject *parent) :
    QAbstractItemModel(parent),
    m_currentSong(-1),
    m_shuffle(false) //TODO: query player for state as soon as API supports it
{
    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReveiced(int,QVariantMap)));
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
    staticMetaObject.invokeMethod(this, "refresh", Qt::QueuedConnection);

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    setRoleNames(roleNames);
}

void Playlist::refresh()
{
//    QVariantMap fieldMap;
//    QVariantList fields;
//    fields.append("title");
//    fields.append("artist");
//    fieldMap.insert("fields", fields);
    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems"); //, fieldMap);
    m_requestMap.insert(id, RequestGetItems);
}

void Playlist::queryItemData(int index)
{
    QVariantMap params;
    QVariantList fields;
    fields.append("title");
    fields.append("artist");
    fields.append("album");
    fields.append("fanart");
    fields.append("thumbnail");
    params.insert("fields", fields);

    QVariantMap limits;
    limits.insert("start", index);
    limits.insert("end", index + 1);
    params.insert("limits", limits);

    int id = XbmcConnection::sendCommand(namespaceString() + ".GetItems", params);
    m_requestMap.insert(id, RequestCurrentData);
}

void Playlist::clear()
{
    beginResetModel();
    m_itemList.clear();
    XbmcConnection::sendCommand("AudioPlaylist.Clear");
    endResetModel();
}

void Playlist::addItems(int artistId, int albumId)
{

    PlaylistItem pItem;
    pItem.setArtistId(artistId);
    pItem.setAlbumId(albumId);
    QVariantMap item;
    item.insert("item", pItem.toMap());

    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
}

void Playlist::addPlaylist(const QString &playlistId)
{
    PlaylistItem pItem;
    pItem.setPlayList(playlistId);
    QVariantMap item;
    item.insert("item", pItem.toMap());

    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
}

void Playlist::addFile(const QString &file)
{
    PlaylistItem pItem;
    pItem.setFile(file);
    QVariantMap item;
    item.insert("item", pItem.toMap());
    XbmcConnection::sendCommand(namespaceString() + ".Add", item);
}

void Playlist::responseReveiced(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestGetItems: {
//        qDebug() << "got GetItems response:" << response;
//        qDebug() << "resetting model";
        beginResetModel();
        m_itemList.clear();
        QVariantList responseList = rsp.toMap().value("items").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            SongItem item;
//            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
//            item.setTitle(itemMap.value("title").toString());
//            item.setArtist(itemMap.value("artist").toString());
//            qDebug() << "adding item:" << item.label();
            m_itemList.append(item);
        }
        endResetModel();
        m_currentSong = rsp.toMap().value("state").toMap().value("current").toInt();
        queryItemData(m_currentSong);
        qDebug() << "set current to" << m_currentSong;
        emit countChanged();
        emit currentChanged();
        break;
    }
    case RequestCurrentData: {
        if(m_itemList.count() > m_currentSong && m_currentSong > -1) {
            SongItem item = m_itemList.at(m_currentSong);
            QVariantList responseList = rsp.toMap().value("items").toList();
            QVariantMap itemMap = responseList.first().toMap();
    //            item.setFanart(itemMap.value("fanart").toString());
            item.setLabel(itemMap.value("label").toString());
            item.setTitle(itemMap.value("title").toString());
            item.setArtist(itemMap.value("artist").toString());
            item.setAlbum(itemMap.value("album").toString());
            item.setFanart(itemMap.value("fanart").toString());
            item.setThumbnail(itemMap.value("thumbnail").toString());
    //            qDebug() << "adding item:" << item.label();
            m_itemList.replace(m_currentSong, item);
            emit currentChanged();
            break;
        }
        }
    }
}

void Playlist::receivedAnnouncement(const QVariantMap &map)
{
    if(map.value("message").toString() == "QueueNextItem") {
    } else if(map.value("message").toString() == "PlaybackStarted") {
    }
}

QModelIndex Playlist::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex Playlist::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int Playlist::rowCount(const QModelIndex &parent) const
{
    return m_itemList.count();
}

int Playlist::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Playlist::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return m_itemList.at(index.row()).label();
    case Qt::DecorationRole:
        return m_itemList.at(index.row()).file();
    }
    return QVariant();
}

SongItem Playlist::at(int index)
{
    return m_itemList.at(index);
}

void Playlist::playItem(int index)
{
    QVariantMap map;
    map.insert("item", index);
    XbmcConnection::sendCommand(namespaceString() + ".Play", map);
    m_currentSong = index;
    emit currentChanged();
}

void Playlist::skipNext()
{
    if(m_currentSong < m_itemList.count() - 1) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipNext");
        m_currentSong++;
        emit currentChanged();
    }
}

void Playlist::skipPrevious()
{
    if(m_currentSong > 0) {
        XbmcConnection::sendCommand(namespaceString() + ".SkipPrevious");
        m_currentSong--;
        emit currentChanged();
    }
}

QString Playlist::currentLabel() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).label();
}

QString Playlist::currentTitle() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).title();
}

QString Playlist::currentArtist() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).artist();
}
QString Playlist::currentFanart() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).fanart();
}

QString Playlist::currentThumbnail() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).thumbnail();
}

QString Playlist::currentAlbum() const
{
    if(m_currentSong == -1 || m_currentSong >= m_itemList.count()) {
        return QString();
    }
    return m_itemList.at(m_currentSong).album();
}

int Playlist::currentTrackNumber() const
{
    return m_currentSong + 1;
}

int Playlist::count() const
{
    return m_itemList.count();
}

bool Playlist::shuffle() const
{
    return m_shuffle;
}

void Playlist::setShuffle(bool shuffle)
{
    if(shuffle) {
        XbmcConnection::sendCommand(namespaceString() + ".Shuffle");
    } else {
        XbmcConnection::sendCommand(namespaceString() + ".UnShuffle");
    }
    if(m_shuffle != shuffle) {
        m_shuffle = shuffle;
        emit shuffleChanged();
    }
}

}
