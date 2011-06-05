#include "videolibrary.h"
#include "xbmcconnection.h"
#include "player.h"
#include "playlist.h"
#include "videoplaylistitem.h"

#include <QDebug>

VideoLibrary::VideoLibrary(Player *player, QObject *parent):
    QAbstractItemModel(parent),
    m_player(player)
{
    m_state = "library";

    // intialize with default stuff
    m_list.append(LibraryItem("Movies", 0));
    m_list.append(LibraryItem("Music Videos", 1));
    m_list.append(LibraryItem("TV-Shows", 2));

    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "itemId");
    setRoleNames(roleNames);

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
}

void VideoLibrary::enterItem(int index)
{

    qDebug() << "entering item" << index << "state is" << m_state;
    if(m_state == "library") {
        switch(index) {
        case 0:
            showMovies();
            break;
        case 1:
            showMusicVideos();
            break;
        case 2:
            showTvShows();
            break;
        }
    } else if(m_state == "movies") {
        m_player->playlist()->clear();
        VideoPlaylistItem item(index);
        m_player->playlist()->addItems(item);
        m_player->playlist()->playItem(0);
    } else if(m_state == "musicvideos") {
        m_player->playlist()->clear();
        VideoPlaylistItem item;
        item.setMusicVideoId(index);
        m_player->playlist()->addItems(item);
        m_player->playlist()->playItem(0);
    } else if(m_state == "tvshows") {
        showSeasons(index);
    } else if(m_state == "seasons") {
        showEpisodes(m_tvShowId, index);
    } else if(m_state == "episodes") {
        m_player->playlist()->clear();
        VideoPlaylistItem item;
        item.setEpisodeId(index);
        m_player->playlist()->addItems(item);
        m_player->playlist()->playItem(0);
    }
}

void VideoLibrary::showLibrary()
{

}

void VideoLibrary::showMovies()
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    QVariantList fields;

    params.insert("fields", fields);


    int id = XbmcConnection::sendCommand("VideoLibrary.GetMovies", params);
    m_requestMap.insert(id, RequestMovies);

    m_state = "movies";
    emit stateChanged();
}

void VideoLibrary::showMusicVideos()
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    int id = XbmcConnection::sendCommand("VideoLibrary.GetMusicVideos", params);
    m_requestMap.insert(id, RequestMusicVideos);
    m_state = "musicvideos";
    emit stateChanged();
}

void VideoLibrary::showTvShows()
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    int id = XbmcConnection::sendCommand("VideoLibrary.GetTVShows", params);
    m_requestMap.insert(id, RequestTvShows);
    m_state = "tvshows";
    emit stateChanged();
}

void VideoLibrary::showSeasons(int tvshowid)
{
    m_tvShowId = tvshowid;

    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    params.insert("tvshowid", tvshowid);

    QVariantList fields;
    fields.append("season");
    params.insert("fields", fields);


    int id = XbmcConnection::sendCommand("VideoLibrary.GetSeasons", params);
    m_requestMap.insert(id, RequestSeasons);
    m_state = "seasons";
    emit stateChanged();
}

void VideoLibrary::showEpisodes(int tvshowid, int episodeid)
{
    QVariantMap params;

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

    params.insert("tvshowid", tvshowid);
    params.insert("season", m_movieList.at(episodeid).id());

    int id = XbmcConnection::sendCommand("VideoLibrary.GetEpisodes", params);
    m_requestMap.insert(id, RequestEpisodes);
    m_state = "episodes";
    emit stateChanged();
}

void VideoLibrary::goUp(int levels)
{
    qDebug() << "Should go up!";

    if(levels <= 0 || m_state == "library") {
        return; // Nothing to do
    }

    if(m_state == "movies" || m_state == "musicvideos" || m_state == "tvshows" ||
            (m_state == "seasons" && levels >= 2) ||
            (m_state == "episodes" && levels >= 3)) {
         m_state = "library";
        emit stateChanged();
        beginResetModel();
        endResetModel();
    }

    if((m_state == "seasons" && levels == 1) ||
            (m_state == "episodes" && levels == 2)) {
        showTvShows();
    }

    if(m_state == "episodes" && levels == 1) {
        showSeasons(m_tvShowId);
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
            qDebug() << "adding item:" << item.label() << ":" << item.id();
            m_movieList.append(item);
        }
        endResetModel();
        }
        break;
    case RequestMusicVideos: {
        beginResetModel();
        m_movieList.clear();
        qDebug() << "got musicvideos:" << rsp;
        QVariantList responseList = rsp.toMap().value("musicvideos").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            MovieItem item(itemMap.value("label").toString(), itemMap.value("musicvideoid").toInt());
            qDebug() << "adding item:" << item.label() << ":" << item.id();
            m_movieList.append(item);
        }
        endResetModel();
        }
        break;
    case RequestTvShows: {
        beginResetModel();
        m_movieList.clear();
        qDebug() << "got tv shows:" << rsp;
        QVariantList responseList = rsp.toMap().value("tvshows").toList();
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            MovieItem item(itemMap.value("label").toString(), itemMap.value("tvshowid").toInt());
            qDebug() << "adding item:" << item.label();
            m_movieList.append(item);
        }
        endResetModel();
        }
        break;
    case RequestSeasons: {
        beginResetModel();
        m_movieList.clear();
        qDebug() << "got seasons:" << rsp;
        QVariantList responseList = rsp.toMap().value("seasons").toList();
        int i = 0;
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            MovieItem item(itemMap.value("label").toString(), itemMap.value("season").toLongLong());
            qDebug() << "adding item:" << item.label();
            m_movieList.append(item);
        }
        endResetModel();
        }
        break;
    case RequestEpisodes: {
        beginResetModel();
        m_movieList.clear();
        qDebug() << "got episodes:" << rsp;
        QVariantList responseList = rsp.toMap().value("episodes").toList();
        int i = 0;
        foreach(const QVariant &itemVariant, responseList) {
            QVariantMap itemMap = itemVariant.toMap();
            MovieItem item(itemMap.value("label").toString(), i++);
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
    if(m_state == "musicvideos") {
        return "Library/Music Videos/";
    }
    if(m_state == "tvshows") {
        return "Library/TV Shows/";
    }
    if(m_state == "seasons") {
        return "Library/TV Shows/Seasons/";
    }
    if(m_state == "episodes") {
        return "Library/TV Shows/Seasons/Episodes";
    }
    return QString("Library/");
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
    if(m_state == "movies" || m_state == "musicvideos" || m_state == "tvshows" || m_state == "seasons" || m_state == "episodes") {
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
    } else if(m_state == "movies" || m_state == "musicvideos" || m_state == "tvshows" || m_state == "seasons" || m_state == "episodes") {
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
