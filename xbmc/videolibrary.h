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

#ifndef VIDEOLIBRARY_H
#define VIDEOLIBRARY_H

#include "libraryitem.h"
#include "movieitem.h"

#include <QAbstractItemModel>

class Player;

class VideoLibrary: public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
//    Q_PROPERTY(int artistFilter READ artistFilter)
//    Q_PROPERTY(int albumFilter READ albumFilter)
    Q_PROPERTY(QString currentDir READ currentDir NOTIFY stateChanged)

public:
    explicit VideoLibrary(Player *player, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString state();
//    int artistFilter();
//    int albumFilter();
    QString currentDir();

signals:
    void stateChanged();

public slots:
    void enterItem(int index);
    void showLibrary();
    void showMusicVideos();
    void showMovies();
    void showTvShows();
    void showSeasons(int tvshowid);
    void showEpisodes(int tvshowid, int episodeid);
    void goUp(int levels);

private:
    enum Request {
        RequestMovies,
        RequestMusicVideos,
        RequestTvShows,
        RequestSeasons,
        RequestEpisodes
    };
    QMap<int, Request> m_requestMap;

    Player *m_player;

    QString m_state;

    QList<LibraryItem> m_list;
    QList<MovieItem> m_movieList;
//    QList<AlbumItem> m_albumList;
//    QList<SongItem> m_songList;

    int m_tvShowId;

private slots:
    void responseReceived(int, const QVariantMap &response);

};

#endif // VIDEOLIBRARY_H
