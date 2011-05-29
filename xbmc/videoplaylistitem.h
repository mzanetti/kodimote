#ifndef VIDEOPLAYLISTITEM_H
#define VIDEOPLAYLISTITEM_H

#include "playlistitem.h"

class VideoPlaylistItem : public PlaylistItem
{
public:
    VideoPlaylistItem(int movieId = -1, int musicVideoId = -1, int episodeid = -1, const QString &file = QString(), const QString &playlist = QString());

    void setMovieId(int movieId);
    int movieId() const;

    void setMusicVideoId(int musicVideoId);
    int musicVideoId() const;

    void setEpisodeId(int episodeId);
    int episodeId() const;

    virtual QVariantMap toMap() const;

private:
    int m_movieId;
    int m_musicVideoId;
    int m_episodeId;
};

#endif // VIDEOPLAYLISTITEM_H
