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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "playlistitem.h"
#include "songitem.h"

#include <QObject>
#include <QVariantMap>
#include <QAbstractItemModel>

namespace Xbmc
{

class Playlist : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int currentTrackNumber READ currentTrackNumber NOTIFY currentChanged)
    Q_PROPERTY(QString currentLabel READ currentLabel NOTIFY currentChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY currentChanged)
    Q_PROPERTY(QString currentAlbum READ currentAlbum NOTIFY currentChanged)
    Q_PROPERTY(QString currentFanart READ currentThumbnail NOTIFY currentChanged)
    Q_PROPERTY(QString currentThumbnail READ currentThumbnail NOTIFY currentChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)

public:
    explicit Playlist(QObject *parent = 0);


    void clear();
    void addItems(int artistId, int albumId);
    void addPlaylist(const QString &playlistId);
    void addFile(const QString &file);

    int count() const;

    bool shuffle() const;
    void setShuffle(bool shuffle);

    int currentTrackNumber() const;
    QString currentLabel() const;
    QString currentTitle() const;
    QString currentArtist() const;
    QString currentAlbum() const;
    QString currentFanart() const;
    QString currentThumbnail() const;
    SongItem currentItem() const;


    SongItem at(int index);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:
    void countChanged();
    void currentChanged();
    void shuffleChanged();
    void repeatChanged();

public slots:
    void refresh();
    void playItem(int index);
    void skipNext();
    void skipPrevious();

private slots:
    void responseReveiced(int id, const QVariantMap &response);
    void receivedAnnouncement(const QVariantMap &map);

protected:
    virtual QString namespaceString() const = 0;

    void queryItemData(int index);

private:
    enum Request {
        RequestGetItems,
        RequestCurrentData
    };

    QMap<int, Request> m_requestMap;
    QList<SongItem> m_itemList;

    int m_currentSong;
    bool m_shuffle;
};

}

#endif // PLAYLIST_H
