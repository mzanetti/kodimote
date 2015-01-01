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

#ifndef SEASONS_H
#define SEASONS_H

#include "kodilibrary.h"

#include <QStandardItem>

class Seasons : public KodiLibrary
{
    Q_OBJECT
public:
    explicit Seasons(int tvhsowId = -1, KodiModel *parent = 0);

    KodiModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

    KodiModel::ThumbnailFormat thumbnailFormat() const { return KodiModel::ThumbnailFormatPortrait; }
    bool allowWatchedFilter() { return true; }

public slots:
    void refresh();

private slots:
    void listReceived(const QVariantMap &rsp);
    void seasonDetailsReceived(const QVariantMap &rsp);
    void receivedAnnouncement(const QVariantMap &map);
    void playcountReceived(const QVariantMap &rsp);

private:
    int m_tvshowid;
    QMap<int, int> m_detailsRequestMap;
    QMap<int, int> m_seasonIndexMapping;
    bool m_refreshing;
};

#endif // SEASONS_H
