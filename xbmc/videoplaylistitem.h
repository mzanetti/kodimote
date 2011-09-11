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

#ifndef VIDEOPLAYLISTITEM_H
#define VIDEOPLAYLISTITEM_H

#include "playlistitem.h"

class VideoPlaylistItem : public PlaylistItem
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString tvShow READ tvShow NOTIFY tvShowChanged)
    Q_PROPERTY(QString season READ season NOTIFY seasonChanged)
    Q_PROPERTY(QString year READ year NOTIFY yearChanged)
    Q_PROPERTY(QString rating READ rating NOTIFY ratingChanged)

public:
    VideoPlaylistItem(int movieId = -1, int musicVideoId = -1, int episodeid = -1);

    void setMovieId(int movieId);
    int movieId() const;

    void setMusicVideoId(int musicVideoId);
    int musicVideoId() const;

    void setTvShowId(int tvShowId);
    int tvShowId() const;

    void setSeasonId(int seasonId);
    int seasonId() const;

    void setEpisodeId(int episodeId);
    int episodeId() const;

    virtual QVariantMap toMap() const;

    void setType(const QString &type);
    QString type() const;
    void setTvShow(const QString &tvShow);
    QString tvShow() const;
    void setSeason(const QString &season);
    QString season() const;
    void setYear(const QString &year);
    QString year() const;
    void setRating(const QString &rating);
    QString rating() const;

signals:
    void tvShowChanged();
    void seasonChanged();
    void typeChanged();
    void yearChanged();
    void ratingChanged();

private:
    int m_movieId;
    int m_musicVideoId;
    int m_tvShowId;
    int m_seasonId;
    int m_episodeId;

    QString m_type;
    QString m_tvShow;
    QString m_season;
    QString m_year;
    QString m_rating;
};

#endif // VIDEOPLAYLISTITEM_H
