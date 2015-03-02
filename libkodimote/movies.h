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

#ifndef MOVIES_H
#define MOVIES_H

#include "kodilibrary.h"

#include <QStandardItem>

class Movies : public KodiLibrary
{
    Q_OBJECT
public:
    explicit Movies(bool recentlyAdded = false, KodiModel *parent = 0);
    ~Movies();

    KodiModel *enterItem(int index);
    void playItem(int index, bool resume = false);
    void addToPlaylist(int index);

    QString title() const;

    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasShortDetails() { return true; }
    Q_INVOKABLE bool hasDetails() { return true; }

    Q_INVOKABLE virtual void download(int index, const QString &path);

    ThumbnailFormat thumbnailFormat() const { return ThumbnailFormatPortrait; }
    MediaFormat mediaFormat() const { return MediaFormatVideo; }
    bool allowWatchedFilter() { return true; }
    QString watchedFilterSetting() { return "showWatchedMovies"; }

public slots:
    void refresh();

private slots:
    void listReceived(const QVariantMap &rsp);
    void detailsReceived(const QVariantMap &rsp);
    void receivedAnnouncement(const QVariantMap &map);

private:
    QMap<int, int> m_detailsRequestMap;
    QMap<int, int> m_idIndexMapping;
    bool m_recentlyAdded;
};

#endif // MOVIES_H
