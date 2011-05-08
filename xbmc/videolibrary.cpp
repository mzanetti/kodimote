#include "videolibrary.h"
#include "xbmcconnection.h"

#include <QDebug>

VideoLibrary::VideoLibrary(Player *player, QObject *parent):
    QAbstractItemModel(parent),
    m_player(player)
{
    m_state = "library";

    // intialize with default stuff
    m_list.append(LibraryItem("Files", 0));
    m_list.append(LibraryItem("Playlists", 1));

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "itemId");
    setRoleNames(roleNames);

    m_albumFilter = -1;
    m_artistFilter = -1;
}

void VideoLibrary::enterItem(int index)
{
    qDebug() << "entering item" << index << "state is" << m_state;
    if(m_state == "library") {
        switch(index) {
        case 0:
            showFiles();
            break;
        case 1:
//            showPlaylists();
            break;
        }
    }
}

void VideoLibrary::showLibrary()
{

}

void VideoLibrary::showFiles()
{
//    qDebug() << "requesting files";
//    QVariantMap params;
//    params.insert("genreid", 1);
//    int id = XbmcConnection::sendCommand("VideoLibrary.GetArtists");//, params);
//    m_requestMap.insert(id, RequestArtists);
}

void VideoLibrary::goUp(int levels)
{

}

void VideoLibrary::responseReceived(int, const QVariantMap &response)
{

}

QString VideoLibrary::state()
{
    return m_state;
}

QString VideoLibrary::currentDir()
{
    return "Library/";
}

QModelIndex VideoLibrary::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex VideoLibrary::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int VideoLibrary::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

int VideoLibrary::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant VideoLibrary::data(const QModelIndex &index, int role) const
{
    if(m_state == "library") {
        if(role == Qt::DisplayRole) {
            return m_list.at(index.row()).label();
        }
        return m_list.at(index.row()).id();
//    } else if(m_state == "artists") {
//        if(role == Qt::DisplayRole) {
//            return m_artistList.at(index.row()).label();
//        }
//        return m_artistList.at(index.row()).id();
//    } else if(m_state == "albums") {
//        if(role == Qt::DisplayRole) {
//            return m_albumList.at(index.row()).label();
//        }
//        return m_albumList.at(index.row()).id();
//    } else if(m_state == "songs") {
//        if(role == Qt::DisplayRole) {
//            return m_songList.at(index.row()).label();
//        }
//        return m_songList.at(index.row()).songId();
    }
    return QVariant();
}
