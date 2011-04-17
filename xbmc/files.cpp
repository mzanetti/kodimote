#include "files.h"
#include "player.h"
#include "playlist.h"
#include "xbmcconnection.h"

#include <QVariant>

namespace Xbmc
{

Files::Files(Player *player, QObject *parent) :
    QAbstractItemModel(parent),
    m_player(player)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "directory");
    setRoleNames(roleNames);


    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    listShares();
}

void Files::listShares()
{
    QVariantMap params;

    QVariant media("music");

    params.insert("media", media);

    int id = XbmcConnection::sendCommand("Files.GetSources", params);
    m_requestMap.insert(id, RequestSources);

}

QModelIndex Files::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex Files::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int Files::rowCount(const QModelIndex &parent) const
{
    return m_filesList.count();
}

int Files::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Files::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole) {
        return m_filesList.at(index.row()).first;
    }
    if(role == Qt::UserRole) {
        return m_filesList.at(index.row()).second;
    }
    return QVariant();
}

void Files::responseReceived(int id, const QVariantMap &response)
{
    if(!m_requestMap.contains(id)) {
        return;
    }

    QVariant rsp = response.value("result");

    switch(m_requestMap.value(id)) {
    case RequestSources: {
        beginResetModel();
        m_sharesMap.clear();
        m_filesList.clear();
        QVariantList shares = rsp.toMap().value("shares").toList();
        foreach(const QVariant &variant, shares) {
            QVariantMap share = variant.toMap();
            m_filesList.append(qMakePair(share.value("label").toString(), share.value("file").toString()));
            m_sharesMap.insert(share.value("file").toString(), share.value("label").toString());
        }
        endResetModel();
        m_currentDir.clear();
        m_currentDirLabel.clear();
        emit dirChanged();
        break;
    }
    case RequestDirectory:
        beginResetModel();
        m_filesList.clear();
        QVariantList shares = rsp.toMap().value("files").toList();
        foreach(const QVariant &variant, shares) {
            QVariantMap share = variant.toMap();
            m_filesList.append(qMakePair(share.value("label").toString(), share.value("file").toString()));
        }
        endResetModel();
        break;
    }
}

void Files::enterDir(const QString &directory)
{
    QString dir = directory;
    if(directory.startsWith("..")) {
        dir = m_currentDir;
        qDebug() << "dir1:" << dir << m_sharesMap;
        for(int i = 0; i < directory.split('/').count(); ++i) {
            if(m_sharesMap.contains(dir)) {
                // on top-level... go back to shares
                listShares();
                return;
            }
            dir.remove(dir.length()-1 , 1);
            dir.remove(dir.lastIndexOf('/') + 1, dir.length() - dir.lastIndexOf('/'));
            qDebug() << "dir2:" << dir;
        }
        qDebug() << "dir3" << dir;
    }
    if(dir.endsWith('/')) {
        QVariantMap params;
        params.insert("directory", dir);

        QVariant media("music");
        params.insert("media", media);

        QVariantList fields;
        fields.append("title");
        fields.append("artist");
        fields.append("albumartist");
        fields.append("genre");
        fields.append("year");
        fields.append("rating");
        fields.append("album");
        fields.append("track");
        fields.append("duration");
        fields.append("comment");
        fields.append("lyrics");
        fields.append("musicbrainztrackid");
        fields.append("musicbrainzartistid");
        fields.append("musicbrainzalbumid");
        fields.append("musicbrainzalbumartistid");
        fields.append("playcount");
        fields.append("fanart");
        fields.append("director");
        fields.append("trailer");
        fields.append("tagline");
        fields.append("plot");
        fields.append("plotoutline");
        fields.append("originaltitle");
        fields.append("lastplayed");
        fields.append("writer");
        fields.append("studio");
        fields.append("mpaa");
        fields.append("cast");
        fields.append("country");
        fields.append("imdbnumber");
        fields.append("premiered");
        fields.append("productioncode");
        fields.append("runtime");
        fields.append("set");
        fields.append("showlink");
        fields.append("streamDetails");
        fields.append("top250");
        fields.append("votes");
        fields.append("writingcredits");
        fields.append("firstaired");
        fields.append("season");
        fields.append("episode");
        fields.append("showtitle");
        fields.append("thumbnail");
        fields.append("file");

        params.insert("fields", fields);

        int id = XbmcConnection::sendCommand("Files.GetDirectory", params);
        m_requestMap.insert(id, RequestDirectory);
        m_currentDir = dir;
        m_currentDirLabel = dir;
        foreach(const QString &sharePath, m_sharesMap.keys()) {
            qDebug() << "sharePath" << sharePath;
            if(m_currentDirLabel.startsWith(sharePath)) {
                m_currentDirLabel.replace(sharePath, m_sharesMap.value(sharePath) + '/');
            }
        }
        emit dirChanged();
    } else {
        qDebug() << "play item here";
        if(dir.startsWith('/')) {
            // regular file: create playlist from entire folder
            //dir.remove(dir.lastIndexOf('/') + 1, dir.length() - dir.lastIndexOf('/'));
            QVariantMap params;

//            QVariantMap playlist;
//            playlist.insert("file", dir);
//            playlist.insert("id", "dummy");
//            params.insert("playlist", playlist);
//            XbmcConnection::sendCommand("Playlist.Create", params);

            m_player->playlist()->clear();
            int newListCount = 0;
            int songToPlay = 0;
            for(int i = 0; i < m_filesList.count(); ++i) {
                QString file = m_filesList.at(i).second;
                if(!file.endsWith('/')) { // only add files (no dirs)
                    m_player->playlist()->addFile(file);
                    if(file == dir) {
                        songToPlay = newListCount;
                    }
                    newListCount++;
                }
            }
            m_player->playlist()->playItem(songToPlay); // -1 because of the ".."
        } else {
            m_player->playlist()->clear();
            m_player->playlist()->addFile(dir);
//            m_player->playlist()->playItem(0);
        }
//        for(int i = 0; i < m_songList.count(); ++i) {
//            if(m_songList.at(i).songId() == index) {
//                break;
//            }
//        }
    }
}

void Files::goUp(int levels)
{
    QString dir;
    for(int i = 0; i < levels; ++i) {
        if(!dir.isEmpty()) {
            dir.append('/');
        }
        dir.append("..");
    }
    enterDir(dir);
}

QString Files::currentDir() const
{
    qDebug() << "currentdir is" << m_currentDirLabel;
    return "Files/" +m_currentDirLabel;
}

}
