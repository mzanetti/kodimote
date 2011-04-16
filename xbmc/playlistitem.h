#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QString>
#include <QVariantMap>

namespace Xbmc
{


class PlaylistItem
{
public:
    PlaylistItem(int albumId = -1, int artistId = -1, const QString &file = QString(), int genreId = -1, const QString &playlist = QString());

    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setFile(const QString &file);
    void setGenreId(int genreId);
    void setPlayList(const QString &playlist);

    int albumId() const;
    int artistId() const;
    QString file() const;
    int genreId() const;
    QString playlistId() const;

    QVariantMap toMap() const;

private:
    int m_albumId;
    int m_artistId;
    QString m_file;
    int m_genreId;
    QString m_playlistId;
};

}
#endif // PLAYLISTITEM_H
