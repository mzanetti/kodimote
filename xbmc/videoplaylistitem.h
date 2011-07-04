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
    VideoPlaylistItem(int movieId = -1, int musicVideoId = -1, int episodeid = -1, const QString &file = QString(), const QString &playlist = QString());

    void setMovieId(int movieId);
    int movieId() const;

    void setMusicVideoId(int musicVideoId);
    int musicVideoId() const;

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
    int m_episodeId;

    QString m_type;
    QString m_tvShow;
    QString m_season;
    QString m_year;
    QString m_rating;
};

#endif // VIDEOPLAYLISTITEM_H
