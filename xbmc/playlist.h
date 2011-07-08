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
#include "player.h"
#include "xbmcmodel.h"

#include <QObject>
#include <QVariantMap>

class Playlist : public XbmcModel
{
    Q_OBJECT

    Q_ENUMS(Repeat)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(PlaylistItem* currentItem READ currentItem NOTIFY currentChanged)
    Q_PROPERTY(int currentTrackNumber READ currentTrackNumber NOTIFY currentChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(Repeat repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

    Q_PROPERTY(Player* player READ player)

public:
    enum Repeat {
        RepeatNone,
        RepeatOne,
        RepeatAll
    };

    explicit Playlist(Player *parent = 0);


    virtual void clear();
    virtual void addItems(const PlaylistItem &item);
    void addPlaylist(const QString &playlistId);
    void addFile(const QString &file);

    int count() const;

    bool shuffle() const;
    void setShuffle(bool shuffle);

    Repeat repeat() const;
    void setRepeat(Repeat repeat);

    int currentTrackNumber() const;

    Player *player() const;

    PlaylistItem* currentItem() const;
    virtual PlaylistItem* at(int index) const = 0;

    XbmcModel *enterItem(int) { return 0;}

signals:
    void countChanged();
    void currentChanged();
    void shuffleChanged();
    void repeatChanged();

public slots:
    virtual void refresh() = 0;
    void playItem(int index);
    void skipNext();
    void skipPrevious();

private slots:
    virtual void responseReveiced(int id, const QVariantMap &response) = 0;
    void receivedAnnouncement(const QVariantMap &map);

protected:
    virtual QString namespaceString() const = 0;

    virtual void queryItemData(int index) = 0;

    enum Request {
        RequestGetItems,
        RequestCurrentData
    };

    QMap<int, Request> m_requestMap;

    mutable int m_currentItem;
    bool m_shuffle;
    Player *m_player;
    Repeat m_repeat;
};

#endif // PLAYLIST_H
