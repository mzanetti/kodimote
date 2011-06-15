/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "files.h"
#include "player.h"
#include "playlist.h"
#include "xbmcconnection.h"

#include <QVariant>

Files::Files(MediaType mediaType, Player *player, QObject *parent) :
    QAbstractItemModel(parent),
    m_player(player),
    m_mediaType(mediaType)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, "label");
    roleNames.insert(Qt::UserRole, "directory");
    setRoleNames(roleNames);

    if(mediaType == MediaTypeAudio) {
        m_playBehavior = PlayBehaviorAll;
    } else {
        m_playBehavior = PlayBehaviorSingle;
    }

    connect(XbmcConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));

    listShares();
}

void Files::setMediaType(MediaType &mediaType)
{
    m_mediaType = mediaType;
    emit mediaTypeChanged();
}

Files::MediaType Files::mediaType() const
{
    return m_mediaType;
}

QString Files::mediaString()
{
    if(m_mediaType == MediaTypeAudio) {
        return "music";
    } else {
        return "video";
    }
}

void Files::listShares()
{
    QVariantMap params;

    QVariant media(mediaString());
    params.insert("media", media);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    sort.insert("ignorearticle", true);
    params.insert("sort", sort);

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

//    qDebug() << "Files reponse:" << response;

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
            QString label = share.value("label").toString();
            QString file = share.value("file").toString();
            if(label.isEmpty()) {
                label = file.right(file.length() - file.lastIndexOf('/') - 1);
            }
            m_filesList.append(qMakePair(label, file));
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

        QVariant media(mediaString());
        params.insert("media", media);

        QVariantList fields;
        fields.append("file");

        params.insert("fields", fields);

        QVariantMap sort;
        sort.insert("method", "label");
        sort.insert("order", "ascending");
        sort.insert("ignorearticle", true);
        params.insert("sort", sort);

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
        qDebug() << "play item here" << directory << dir;
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
            if(m_playBehavior == PlayBehaviorAll) {
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
                m_player->playlist()->playItem(songToPlay);
            } else {
                m_player->playlist()->addFile(dir);
                m_player->playlist()->playItem(0);
            }
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
