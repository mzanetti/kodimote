#include "videoplaylistitem.h"

VideoPlaylistItem::VideoPlaylistItem(int movieId, int musicVideoId, int episodeId):
    m_movieId(movieId),
    m_musicVideoId(musicVideoId),
    m_episodeId(episodeId),
    m_tvShowId(-1),
    m_seasonId(-1)
{
}

void VideoPlaylistItem::setMovieId(int movieId)
{
    m_movieId = movieId;
}

int VideoPlaylistItem::movieId() const
{
    return m_movieId;
}

void VideoPlaylistItem::setMusicVideoId(int musicVideoId)
{
    m_musicVideoId = musicVideoId;
}

int VideoPlaylistItem::musicVideoId() const
{
    return m_musicVideoId;
}

void VideoPlaylistItem::setTvShowId(int tvShowId)
{
    m_tvShowId = tvShowId;
}

int VideoPlaylistItem::tvShowId() const
{
    return m_tvShowId;
}

void VideoPlaylistItem::setSeasonId(int seasonId)
{
    m_seasonId = seasonId;
}

int VideoPlaylistItem::seasonId() const
{
    return m_seasonId;
}

void VideoPlaylistItem::setEpisodeId(int episodeId)
{
    m_episodeId = episodeId;
}

int VideoPlaylistItem::episodeId() const
{
    return m_episodeId;
}

QVariantMap VideoPlaylistItem::toMap() const
{
    QVariantMap map = PlaylistItem::toMap();
    if(m_movieId != -1) {
        map.insert("movieid", m_movieId);
    }
    if(m_musicVideoId != -1) {
        map.insert("musicvideoid", m_musicVideoId);
    }
    if(m_tvShowId != -1) {
        map.insert("tvshowid", m_tvShowId);
    }
    if(m_seasonId != -1) {
        map.insert("seasonid", m_seasonId);
    }
    if(m_episodeId != -1) {
        map.insert("episodeid", m_episodeId);
    }
    return map;
}

void VideoPlaylistItem::setType(const QString &type)
{
    m_type = type;
    emit typeChanged();
}

QString VideoPlaylistItem::type() const
{
    return m_type;
}

void VideoPlaylistItem::setTvShow(const QString &tvShow)
{
    m_tvShow = tvShow;
    emit tvShowChanged();
}

QString VideoPlaylistItem::tvShow() const
{
    return m_tvShow;
}

void VideoPlaylistItem::setSeason(const QString &season)
{
    m_season = season;
    emit seasonChanged();
}

QString VideoPlaylistItem::season() const
{
    return m_season;
}

void VideoPlaylistItem::setYear(const QString &year)
{
    m_year = year;
    emit yearChanged();
}

QString VideoPlaylistItem::year() const
{
    return m_year;
}

void VideoPlaylistItem::setRating(const QString &rating)
{
    m_rating = rating;
    emit ratingChanged();
}

QString VideoPlaylistItem::rating() const
{
    return m_rating;
}
