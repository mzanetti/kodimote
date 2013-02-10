/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "libraryitem.h"

#include "xbmc.h"
#include "imagecache.h"

LibraryItem::LibraryItem(const QString &title, const QString &subTitle):
    XbmcModelItem(title, subTitle),
    m_fileName(QString()),
    m_thumbnail(QString()),
    m_fileType("directory"),
    m_playable(false),
    m_artistId(-1),
    m_albumId(-1),
    m_songId(-1),
    m_musicvideoId(-1),
    m_tvshowId(-1),
    m_seasonId(-1),
    m_episodeId(-1),
    m_movieId(-1),
    m_channelgroupId(-1),
    m_channelId(-1),
    m_plot(QString()),
    m_rating(-1),
    m_season(-1),
    m_episode(-1),
    m_firstAired(QString()),
    m_genre(QString()),
    m_year(QString()),
    m_director(QString()),
    m_tagline(QString()),
    m_mpaa(QString()),
    m_description(QString()),
    m_instrument(QString()),
    m_style(QString()),
    m_mood(QString()),
    m_born(QString()),
    m_formed(QString()),
    m_died(QString()),
    m_disbanded(QString()),
    m_duration(QTime()),
    m_comment(QString()),
    m_playcount(-1),
    m_cast(QString())
{
}

QString LibraryItem::type() const
{
    return m_type;
}

void LibraryItem::setType(const QString &type)
{
    m_type = type;
    emit typeChanged();
}

QVariant LibraryItem::data(int role) const
{
    switch(role) {
    case XbmcModel::RoleFileName:
        return m_fileName;
    case XbmcModel::RoleThumbnail:
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
    case XbmcModel::RoleChannelGroupId:
        return m_channelgroupId;
    case XbmcModel::RoleChannelId:
        return m_channelId;
    case XbmcModel::RolePlot:
        return m_plot;
    case XbmcModel::RoleRating:
        return m_rating;
    case XbmcModel::RoleSeason:
        return m_season;
    case XbmcModel::RoleEpisode:
        return m_episode;
    case XbmcModel::RoleFirstAired:
        return m_firstAired;
    case XbmcModel::RoleGenre:
        return m_genre;
    case XbmcModel::RoleYear:
        return m_year;
    case XbmcModel::RoleDirector:
        return m_director;
    case XbmcModel::RoleTagline:
        return m_tagline;
    case XbmcModel::RoleMpaa:
        return m_mpaa;
    case XbmcModel::RoleDescription:
        return m_description;
    case XbmcModel::RoleInstrument:
        return m_instrument;
    case XbmcModel::RoleStyle:
        return m_style;
    case XbmcModel::RoleMood:
        return m_mood;
    case XbmcModel::RoleBorn:
        return m_born;
    case XbmcModel::RoleFormed:
        return m_formed;
    case XbmcModel::RoleDied:
        return m_died;
    case XbmcModel::RoleDisbanded:
        return m_disbanded;
    case XbmcModel::RoleDuration:
        return m_duration;
    case XbmcModel::RoleComment:
        return m_disbanded;
    case XbmcModel::RolePlaycount:
        return m_playcount;
    case XbmcModel::RoleCast:
        return m_cast;
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
    qDebug() << "thumbnail" << m_thumbnail;
    if (m_thumbnail.isEmpty()) {
        return QString();
    }

    if(Xbmc::instance()->imageCache()->contains(m_thumbnail, 1)) {
        return Xbmc::instance()->imageCache()->cachedFile(m_thumbnail, 1);
    }
    // scaleTo size optimized for big representations.
    int id = Xbmc::instance()->imageCache()->fetch(m_thumbnail, const_cast<LibraryItem*>(this), "imageFetched", QSize(1000, 1000), 1);
    m_imageFetchJobs.insert(id, ImageTypeThumbnail);
    return QString();
}

void LibraryItem::setThumbnail(const QString &thumbnail)
{
    m_thumbnail = thumbnail;
    emit thumbnailChanged();
}

QString LibraryItem::fanart() const
{
    if(Xbmc::instance()->imageCache()->contains(m_fanart, 1)) {
        return Xbmc::instance()->imageCache()->cachedFile(m_fanart, 1);
    }
    int id = Xbmc::instance()->imageCache()->fetch(m_fanart, const_cast<LibraryItem*>(this), "imageFetched", QSize(1000, 1000), 1);
    m_imageFetchJobs.insert(id, ImageTypeFanart);
    return QString();
}

void LibraryItem::setFanart(const QString &fanart)
{
    m_fanart = fanart;
    emit fanartChanged();
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

QString LibraryItem::artist() const
{
    return m_artist;
}

void LibraryItem::setArtist(const QString &artist)
{
    m_artist = artist;
    emit artistChanged();
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

QString LibraryItem::album() const
{
    return m_album;
}

void LibraryItem::setAlbum(const QString &album)
{
    m_album = album;
    emit albumChanged();
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

QString LibraryItem::tvShow() const
{
    return m_tvShow;
}

void LibraryItem::setTvShow(QString tvShow)
{
    m_tvShow = tvShow;
    emit tvShowChanged();
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

int LibraryItem::channelgroupId() const
{
    return m_channelgroupId;
}

void LibraryItem::setChannelgroupId(int channelgroupId)
{
    m_channelgroupId = channelgroupId;
    emit channelgroupIdChanged();
}

int LibraryItem::channelId() const
{
    return m_channelId;
}

void LibraryItem::setChannelId(int channelId)
{
    m_channelId = channelId;
    emit channelIdChanged();
}

QString LibraryItem::plot() const
{
    return m_plot;
}

void LibraryItem::setPlot(const QString &plot)
{
    m_plot = plot;
    emit plotChanged();
}

int LibraryItem::rating() const
{
    return m_rating;
}

void LibraryItem::setRating(int rating)
{
    m_rating = rating;
    emit ratingChanged();
}

int LibraryItem::season() const
{
    return m_season;
}

void LibraryItem::setSeason(int season)
{
    m_season = season;
    emit seasonChanged();
}

int LibraryItem::episode() const
{
    return m_episode;
}

void LibraryItem::setEpisode(int episode)
{
    m_episode = episode;
    emit episodeChanged();
}

QString LibraryItem::firstAired() const
{
    return m_firstAired;
}

void LibraryItem::setFirstAired(const QString &firstAired)
{
    m_firstAired = firstAired;
    emit firstAiredChanged();
}

QString LibraryItem::genre() const
{
    return m_genre;
}

void LibraryItem::setGenre(const QString &genre)
{
    m_genre = genre;
    emit genreChanged();
}

QString LibraryItem::year() const
{
    return m_year;
}

void LibraryItem::setYear(const QString &year)
{
    m_year = year;
    emit yearChanged();
}

QString LibraryItem::director() const
{
    return m_director;
}

void LibraryItem::setDirector(const QString &director)
{
    m_director = director;
    emit directorChanged();
}

QString LibraryItem::tagline() const
{
    return m_tagline;
}

void LibraryItem::setTagline(const QString &tagline)
{
    m_tagline = tagline;
    emit taglineChanged();
}

QString LibraryItem::mpaa() const
{
    return m_mpaa;
}

void LibraryItem::setMpaa(const QString &mpaa)
{
    m_mpaa = mpaa;
    emit mpaaChanged();
}

QString LibraryItem::description() const
{
    return m_description;
}

void LibraryItem::setDescription(const QString &description)
{
    m_description = description;
    emit descriptionChanged();
}

QString LibraryItem::instrument() const
{
    return m_instrument;
}

void LibraryItem::setInstrument(const QString &instrument)
{
    m_instrument = instrument;
    emit instrumentChanged();
}

QString LibraryItem::style() const
{
    return m_style;
}

void LibraryItem::setStyle(const QString &style)
{
    m_style = style;
    emit styleChanged();
}

QString LibraryItem::mood() const
{
    return m_mood;
}

void LibraryItem::setMood(const QString &mood)
{
    m_mood = mood;
    emit moodChanged();
}

QString LibraryItem::born() const
{
    return m_born;
}

void LibraryItem::setBorn(const QString &born)
{
    m_born = born;
    emit bornChanged();
}

QString LibraryItem::formed() const
{
    return m_formed;
}

void LibraryItem::setFormed(const QString &formed)
{
    m_formed = formed;
    emit formedChanged();
}

QString LibraryItem::died() const
{
    return m_died;
}

void LibraryItem::setDied(const QString &died)
{
    m_died = died;
    emit diedChanged();
}

QString LibraryItem::disbanded() const
{
    return m_disbanded;
}

void LibraryItem::setDisbanded(const QString &disbanded)
{
    m_disbanded = disbanded;
    emit disbandedChanged();
}

QTime LibraryItem::duration() const
{
    return m_duration;
}

QString LibraryItem::durationString() const
{
    if(m_duration.hour() > 0) {
        return m_duration.toString("hh:mm:ss");
    }
    return m_duration.toString("mm:ss");
}

int LibraryItem::durationInSecs() const
{
    QTime nullTime;
    return nullTime.msecsTo(m_duration) / 1000;
}

void LibraryItem::setDuration(const QTime &duration)
{
    m_duration = duration;
    emit durationChanged();
}

QString LibraryItem::comment() const
{
    return m_comment;
}

void LibraryItem::setComment(const QString &comment)
{
    m_comment = comment;
    emit commentChanged();
}

int LibraryItem::playcount() const
{
    return m_playcount;
}

void LibraryItem::setPlaycount(int playcount)
{
    m_playcount = playcount;
    emit playcountChanged();
}

QString LibraryItem::cast() const
{
    return m_cast;
}

void LibraryItem::setCast(const QString &cast)
{
    m_cast = cast;
    emit castChanged();
}

void LibraryItem::imageFetched(int id)
{
    if (m_imageFetchJobs.contains(id)) {
        switch(m_imageFetchJobs.take(id)) {
        case ImageTypeThumbnail:
            emit thumbnailChanged();
            break;
        case ImageTypeFanart:
            emit fanartChanged();
            break;
        }
    }
}

