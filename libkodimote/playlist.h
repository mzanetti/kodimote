/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
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
#include "kodilibrary.h"

#include <QObject>
#include <QVariantMap>

class Playlist : public KodiModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(PlaylistItem* currentItem READ currentItem NOTIFY currentChanged)
    Q_PROPERTY(int currentTrackNumber READ currentTrackNumber NOTIFY currentChanged)

    Q_PROPERTY(Player* player READ player)

public:
    explicit Playlist(Player *parent = 0);

    Q_INVOKABLE virtual void clear();
    virtual void addItems(const PlaylistItem &item);
    Q_INVOKABLE void removeItem(int index);

    int count() const;

    int currentTrackNumber() const;

    Player *player() const;

    PlaylistItem* currentItem() const;
    virtual PlaylistItem* at(int index) const = 0;

    virtual int playlistId() const = 0;

signals:
    void countChanged();
    void currentChanged();

public slots:
    virtual void refresh() = 0;
//    void playItem(int index);
    void setCurrentIndex(int index);

private slots:
    void receivedAnnouncement(const QVariantMap &map);

protected:

    virtual void queryItemData(int index) = 0;

    mutable int m_currentItem;
    Player *m_player;
};

#endif // PLAYLIST_H
