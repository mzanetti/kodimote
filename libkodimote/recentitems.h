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

#ifndef RECENTLYADDED_H
#define RECENTLYADDED_H

#include "kodilibrary.h"

#include <QStandardItem>

class RecentItems : public KodiLibrary
{
    Q_OBJECT
public:
    enum RecentlyWhat {
        RecentlyPlayed,
        RecentlyAdded
    };

    explicit RecentItems(MediaFormat mediaFormat, RecentlyWhat what, KodiLibrary *parent);
    
    KodiModel *enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    QString title() const;

    bool allowSearch();

    ThumbnailFormat thumbnailFormat() const { return ThumbnailFormatNone; }
    MediaFormat mediaFormat() const {  return m_mediaFormat; }

public slots:
    void refresh();

private:
    MediaFormat m_mediaFormat;
    RecentlyWhat m_what;
};

#endif // RECENTLYADDED_H
