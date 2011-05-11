#include "videolibrary.h"
#include "xbmcconnection.h"

#include <QDebug>

VideoLibrary::VideoLibrary(Player *player, QObject *parent):
    QAbstractItemModel(parent),
    m_player(player)
{
    m_state = "library";

    // intialize with default stuff
    m_list.append(LibraryItem("Movies", 0));
    m_list.append(LibraryItem("Playlists", 1));

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "itemId");
    setRoleNames(roleNames);

    m_albumFilter = -1;
    m_artistFilter = -1;

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    int id = XbmcConnection::sendCommand("VideoLibrary.GetMovies", params);
    m_requestMap.insert(id, RequestMovies);
}

void VideoLibrary::enterItem(int index)
{
    qDebug() << "entering item" << index << "state is" << m_state;
    if(m_state == "library") {
        switch(index) {
        case 0:
            m_state = "movies";
            emit stateChanged();
            beginResetModel();
            endResetModel();
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
    qDebug() << "Should go up!";

    if(levels <= 0 || m_state == "library") {
        return; // Nothing to do
    }

    if(m_state == "movies") {
        m_state = "library";
        emit stateChanged();
        beginResetModel();
        endResetModel();
    }
}

void VideoLibrary::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    qDebug() << "VideoLibrary response received" << response;

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestMovies: {
        beginResetModel();
        m_movieList.clear();
        qDebug() << "got movies:" << rsp;
        QVariantList responseList = rsp.toMap().value("movies").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            MovieItem item(itemMap.value("label").toString(), itemMap.value("movieid").toInt());
            qDebug() << "adding item:" << item.label();
            m_movieList.append(item);
        }
        endResetModel();
        }
        break;
    }
}

QString VideoLibrary::state()
{
    return m_state;
}

QString VideoLibrary::currentDir()
{
    if(m_state == "library") {
        return "Library/";
    }
    if(m_state == "movies") {
        return "Library/Movies/";
    }
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
    if(m_state == "library") {
        return m_list.count();
    }
    if(m_state == "movies") {
        return m_movieList.count();
    }
    return 0;
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
    } else if(m_state == "movies") {
        if(role == Qt::DisplayRole) {
            return m_movieList.at(index.row()).label();
        }
        return m_movieList.at(index.row()).id();
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
