#ifndef AUDIOPLAYLISTITEM_H
#define AUDIOPLAYLISTITEM_H

#include "playlistitem.h"

class AudioPlaylistItem : public PlaylistItem
{
    Q_OBJECT
    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)

public:
    AudioPlaylistItem(int albumId = -1, int artistId = -1, int genreId = -1);

    // Those will be filled in on toMap()
    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setGenreId(int genreId);

    int albumId() const;
    int artistId() const;
    int genreId() const;

    virtual QVariantMap toMap() const;

    void setArtist(const QString &artist);
    void setAlbum(const QString &album);

    QString artist() const;
    QString album() const;

signals:
    void albumChanged();
    void artistChanged();

private:
    int m_albumId;
    int m_artistId;
    int m_genreId;

    QString m_artist;
    QString m_album;
};

#endif // AUDIOPLAYLISTITEM_H
