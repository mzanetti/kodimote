#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include "xbmcmodel.h"

#include <QObject>
#include <QStandardItem>

class LibraryItem : public XbmcModelItem
{
    Q_OBJECT

    Q_PROPERTY(QString filename READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString filetype READ fileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(bool playable READ playable WRITE setPlayable NOTIFY playableChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
    Q_PROPERTY(int songId READ songId WRITE setSongId NOTIFY songIdChanged)
    Q_PROPERTY(int musicvideoId READ musicvideoId WRITE setMusicvideoId NOTIFY musicvideoIdChanged)
    Q_PROPERTY(int tvshowId READ tvshowId WRITE setTvshowId NOTIFY tvshowIdChanged)
    Q_PROPERTY(int seasonId READ seasonId WRITE setSeasonId NOTIFY seasonIdChanged)
    Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)

public:
    explicit LibraryItem(const QString &title = QString(), const QString &subTitle = QString());

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString thumbnail() const;
    void setThumbnail(const QString &thumbnail);

    QString fileType() const;
    void setFileType(const QString &fileType);

    bool playable() const;
    void setPlayable(bool playable);

    int artistId() const;
    void setArtistId(int artistId);

    int albumId() const;
    void setAlbumId(int albumId);

    int songId() const;
    void setSongId(int songId);

    int musicvideoId() const;
    void setMusicvideoId(int musicvideoId);

    int tvshowId() const;
    void setTvshowId(int tvshowId);

    int seasonId() const;
    void setSeasonId(int seasonId);

    int episodeId() const;
    void setEpisodeId(int episodeId);

    int movieId() const;
    void setMovieId(int movieId);

    virtual QVariant data(int role) const;

signals:
    void fileNameChanged();
    void thumbnailChanged();
    void fileTypeChanged();
    void playableChanged();
    void artistIdChanged();
    void albumIdChanged();
    void songIdChanged();
    void musicvideoIdChanged();
    void tvshowIdChanged();
    void seasonIdChanged();
    void episodeIdChanged();
    void movieIdChanged();

private:
    QString m_fileName;
    QString m_thumbnail;
    QString m_fileType;
    bool m_playable;
    int m_artistId;
    int m_albumId;
    int m_songId;
    int m_musicvideoId;
    int m_tvshowId;
    int m_seasonId;
    int m_episodeId;
    int m_movieId;
};

#endif // LIBRARYITEM_H
