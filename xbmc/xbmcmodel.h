/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
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

#include "xbmcmodelitem.h"

#include <QAbstractItemModel>
#include <QDebug>

class XbmcModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY layoutChanged)
public:
    enum Roles {
        RoleTitle = Qt::DisplayRole,
        RoleFileType = Qt::UserRole + 1,
        RoleSubtitle = Qt::UserRole + 2,
        RoleDuration = Qt::UserRole + 3,
        RolePlayable = Qt::UserRole + 4,

        RoleSongId = Qt::UserRole + 100,
        RoleAlbumId = Qt::UserRole + 101,
        RoleArtistId = Qt::UserRole + 102,
        RoleFileName = Qt::UserRole + 103,
        RoleMovieId = Qt::UserRole + 104,
        RoleTvShowId = Qt::UserRole + 105,
        RoleSeasonId = Qt::UserRole + 106,
        RoleEpisodeId = Qt::UserRole + 107,
        RoleMusicVideoId = Qt::UserRole + 108,
        RoleThumbnail = Qt::UserRole + 109,
        RoleSortingTitle = Qt::UserRole + 110,
        RolePlot = Qt::UserRole + 111,
        RoleRating = Qt::UserRole + 112,
        RoleSeason = Qt::UserRole + 113,
        RoleEpisode = Qt::UserRole + 114,
        RoleFirstAired = Qt::UserRole + 115,
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
        RoleCast
    };

    explicit XbmcModel(XbmcModel *parent = 0);
    virtual ~XbmcModel();
    Q_INVOKABLE XbmcModel *parentModel() const;

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

signals:
    void titleChanged();
    void layoutChanged();

protected:
    XbmcModel *m_parentModel;
    QList<XbmcModelItem*> m_list;
};

#endif // XBMCMODEL_H
