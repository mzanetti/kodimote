#include "libraryitem.h"
#include "xbmc.h"

LibraryItem::LibraryItem(const QString &title, const QString &subTitle):
    XbmcModelItem(title, subTitle),
    m_fileName(""),
    m_thumbnail(""),
    m_fileType("directory"),
    m_playable(false),
    m_artistId(-1),
    m_albumId(-1),
    m_songId(-1),
    m_musicvideoId(-1),
    m_tvshowId(-1),
    m_seasonId(-1),
    m_episodeId(-1),
    m_movieId(-1)
{
}

QVariant LibraryItem::data(int role) const
{
    switch(role) {
    case XbmcModel::RoleFileName:
        return m_fileName;
    case XbmcModel::RoleThumbnail:
        if(!m_thumbnail.isEmpty()) {
            return Xbmc::instance()->vfsPath() + m_thumbnail;
        }
        return m_thumbnail;
    case XbmcModel::RoleFileType:
        return m_fileType;
    case XbmcModel::RolePlayable:
        return m_playable;
    case XbmcModel::RoleArtistId:
        return m_artistId;
    case XbmcModel::RoleAlbumId:
        return m_albumId;
    case XbmcModel::RoleSongId:
        return m_songId;
    case XbmcModel::RoleMusicVideoId:
        return m_musicvideoId;
    case XbmcModel::RoleTvShowId:
        return m_tvshowId;
    case XbmcModel::RoleSeasonId:
        return m_seasonId;
    case XbmcModel::RoleEpisodeId:
        return m_episodeId;
    case XbmcModel::RoleMovieId:
        return m_movieId;
    }

    return XbmcModelItem::data(role);
}

QString LibraryItem::fileName() const
{
    return m_fileName;
}

void LibraryItem::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    emit fileNameChanged();
}

QString LibraryItem::thumbnail() const
{
    return m_thumbnail;
}

void LibraryItem::setThumbnail(const QString &thumbnail)
{
    m_thumbnail = thumbnail;
    emit thumbnailChanged();
}

QString LibraryItem::fileType() const
{
    return m_fileType;
}

void LibraryItem::setFileType(const QString &fileType)
{
    m_fileType = fileType;
    emit fileTypeChanged();
}

bool LibraryItem::playable() const
{
    return m_playable;
}

void LibraryItem::setPlayable(bool playable)
{
    m_playable = playable;
    emit playableChanged();
}

int LibraryItem::artistId() const
{
    return m_artistId;
}

void LibraryItem::setArtistId(int artistId)
{
    m_artistId = artistId;
    emit artistIdChanged();
}

int LibraryItem::albumId() const
{
    return m_albumId;
}

void LibraryItem::setAlbumId(int albumId)
{
    m_albumId = albumId;
    emit albumIdChanged();
}

int LibraryItem::songId() const
{
    return m_songId;
}

void LibraryItem::setSongId(int songId)
{
    m_songId = songId;
    emit songIdChanged();
}

int LibraryItem::musicvideoId() const
{
    return m_musicvideoId;
}

void LibraryItem::setMusicvideoId(int musicvideoId)
{
    m_musicvideoId = musicvideoId;
    emit musicvideoIdChanged();
}

int LibraryItem::tvshowId() const
{
    return m_tvshowId;
}

void LibraryItem::setTvshowId(int tvshowId)
{
    m_tvshowId = tvshowId;
    emit tvshowIdChanged();
}

int LibraryItem::seasonId() const
{
    return m_seasonId;
}

void LibraryItem::setSeasonId(int seasonId)
{
    m_seasonId = seasonId;
    emit seasonIdChanged();
}

int LibraryItem::episodeId() const
{
    return m_episodeId;
}

void LibraryItem::setEpisodeId(int episodeId)
{
    m_episodeId = episodeId;
    emit episodeIdChanged();
}

int LibraryItem::movieId() const
{
    return m_movieId;
}

void LibraryItem::setMovieId(int movieId)
{
    m_movieId = movieId;
    emit movieIdChanged();
}
