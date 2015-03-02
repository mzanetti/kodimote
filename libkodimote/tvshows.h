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

#ifndef TVSHOWS_H
#define TVSHOWS_H

#include "kodilibrary.h"

#include <QStandardItem>

class TvShows : public KodiLibrary
{
    Q_OBJECT
public:
    explicit TvShows(KodiModel *parent = 0);
    ~TvShows();

    KodiModel *enterItem(int index);
    void playItem(int index, bool resume = false);
    void addToPlaylist(int index);

    QString title() const;

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

    ThumbnailFormat thumbnailFormat() const { return ThumbnailFormatLandscape; }
    MediaFormat mediaFormat() const { return MediaFormatVideo; }
    bool allowWatchedFilter() { return true; }
    QString watchedFilterSetting() { return "showWatchedTvShows"; }

public slots:
    void refresh();

private slots:
    void showDetailsReceived(const QVariantMap &rsp);
    void showsReceived(const QVariantMap &rsp);
    void receivedAnnouncement(const QVariantMap &map);
    void playcountReceived(const QVariantMap &rsp);

private:
    QMap<int, int> m_detailsRequestMap;
    QMap<int, int> m_idIndexMapping;
    bool m_refreshing;
};

#endif // TVSHOWS_H
