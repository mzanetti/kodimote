/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef XBMCMODEL_H
#define XBMCMODEL_H

#include "kodimodelitem.h"

#include <QAbstractItemModel>
#include <QDebug>

class KodiModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS(ThumbnailFormat)
    Q_ENUMS(LockMode)
    Q_ENUMS(MediaFormat)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY layoutChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(bool ignoreArticle READ ignoreArticle WRITE setIgnoreArticle NOTIFY ignoreArticleChanged)
    Q_PROPERTY(ThumbnailFormat thumbnailFormat READ thumbnailFormat NOTIFY thumbnailFormatChanged)
    Q_PROPERTY(MediaFormat mediaFormat READ mediaFormat CONSTANT)
    Q_PROPERTY(bool allowSearch READ allowSearch NOTIFY allowSearchChanged)
    Q_PROPERTY(bool allowWatchedFilter READ allowWatchedFilter NOTIFY allowWatchedFilterChanged)

public:
    enum Roles {
        RoleTitle = Qt::DisplayRole,
        RoleFileType = Qt::UserRole + 1,
        RoleSubtitle,
        RoleDuration,
        RolePlayable,
        RoleSongId,
        RoleGenreId,
        RoleAlbumId,
        RoleArtistId,
        RoleFileName,
        RoleMovieId,
        RoleTvShowId,
        RoleSeasonId,
        RoleEpisodeId,
        RoleMusicVideoId,
        RoleChannelGroupId,
        RoleChannelId,
        RoleRecordingId,
        RoleThumbnail,
        RoleLargeThumbnail,
        RoleSortingTitle,
        RolePlot,
        RoleRating,
        RoleSeason,
        RoleEpisode,
        RoleFirstAired,
        RoleGenre,
        RoleYear,
        RoleDirector,
        RoleTagline,
        RoleMpaa,
        RoleDescription,
        RoleInstrument,
        RoleStyle,
        RoleMood,
        RoleBorn,
        RoleFormed,
        RoleDied,
        RoleDisbanded,
        RoleComment,
        RolePlaycount,
        RoleCast,
        RolePlayingState,
        RoleLockMode
    };

    enum ThumbnailFormat {
        ThumbnailFormatNone,
        ThumbnailFormatSquare,
        ThumbnailFormatLandscape,
        ThumbnailFormatPortrait,
        ThumbnailFormat43
    };

    enum MediaFormat {
        MediaFormatUnknown,
        MediaFormatAudio,
        MediaFormatVideo,
        MediaFormatPictures
    };

    enum ItemId {
        ItemIdInvalid = -1,
        ItemIdRecentlyAdded = -2,
        ItemIdRecentlyPlayed = -3
    };

    enum LockMode {
        LockModeNone = 0,
        LockModeNumeric = 1,
        LockModeGamePad = 2,
        LockModeAlphaNumeric = 3
    };

    explicit KodiModel(KodiModel *parent);
    explicit KodiModel(QObject *parent = 0);
    virtual ~KodiModel();
    Q_INVOKABLE KodiModel *parentModel() const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    Q_INVOKABLE virtual QString title() const = 0;

    Q_INVOKABLE int findItem(const QString &string, bool caseSensitive = false);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent)
        return m_list.count();
    }

    QHash<int, QByteArray> roleNames() const;

    bool busy() const;
    void setBusy(bool busy);

    bool ignoreArticle() const;
    void setIgnoreArticle(bool ignoreArticle);

    virtual ThumbnailFormat thumbnailFormat() const { return ThumbnailFormatSquare; }
    virtual MediaFormat mediaFormat() const { return MediaFormatUnknown; }
    virtual bool allowSearch() { return true; }
    virtual bool allowWatchedFilter() { return false; }

    Q_INVOKABLE void imageFetched(int id);
public slots:
    virtual void refresh() = 0;

signals:
    void titleChanged();
    void layoutChanged();
    void busyChanged();
    void ignoreArticleChanged();
    void thumbnailFormatChanged();
    void allowSearchChanged();
    void allowWatchedFilterChanged();

protected:
    KodiModel *m_parentModel;
    QList<KodiModelItem*> m_list;

private:
    bool m_busy;
    bool m_ignoreArticle;

    mutable QHash<int, int> m_imageFetchJobs; // This is a cache... needs to be modified in data() which is const
};

#endif // XBMCMODEL_H
