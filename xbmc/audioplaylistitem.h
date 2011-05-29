#ifndef AUDIOPLAYLISTITEM_H
#define AUDIOPLAYLISTITEM_H

#include "playlistitem.h"

class AudioPlaylistItem : public PlaylistItem
{
public:
    AudioPlaylistItem(int albumId = -1, int artistId = -1, const QString &file = QString(), int genreId = -1, const QString &playlist = QString());

    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setGenreId(int genreId);

    int albumId() const;
    int artistId() const;
    int genreId() const;

    virtual QVariantMap toMap() const;

private:
    int m_albumId;
    int m_artistId;
    int m_genreId;
};

#endif // AUDIOPLAYLISTITEM_H
