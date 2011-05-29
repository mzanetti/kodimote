#include "videoplaylistitem.h"

VideoPlaylistItem::VideoPlaylistItem(int movieId, int musicVideoId, int episodeId, const QString &file, const QString &playlist):
    PlaylistItem(file, playlist),
    m_movieId(movieId),
    m_musicVideoId(musicVideoId),
    m_episodeId(episodeId)
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
    if(m_episodeId != -1) {
        map.insert("episodeid", m_episodeId);
    }
    return map;
}
