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

#include "kodimodel.h"
#include "kodi.h"
#include "imagecache.h"
#include "player.h"

#include <QDebug>

KodiModel::KodiModel(KodiModel *parent) :
    QAbstractItemModel(parent),
    m_parentModel(parent),
    m_busy(true),
    m_ignoreArticle(false)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif
}

KodiModel::KodiModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_parentModel(0),
    m_busy(true),
    m_ignoreArticle(false)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif
}

KodiModel::~KodiModel()
{
    qDebug() << "deleting model";
    while(!m_list.isEmpty()) {
        m_list.takeFirst()->deleteLater();
    }
}

KodiModel *KodiModel::parentModel() const
{
    return m_parentModel;
}

QVariant KodiModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.count()) {
        return QVariant();
    }
    // Lets add a cache here
    if(role == RoleThumbnail) {
        QString thumbnail = m_list.at(index.row())->data(role).toString();
        if(thumbnail.isEmpty()) {
            return QString();
        }
        QString cachedFile;
        if(Kodi::instance()->imageCache()->contains(thumbnail, 0, cachedFile)) {
            return cachedFile;
        }
        // Size optimized for list view icons.
        int job = Kodi::instance()->imageCache()->fetch(thumbnail, const_cast<KodiModel*>(this), "imageFetched", QSize(152, 120), 0);
        m_imageFetchJobs.insert(job, index.row());
        return QString("loading");
    }
    if(role == RoleLargeThumbnail) {
        QString thumbnail = m_list.at(index.row())->data(RoleThumbnail).toString();
        if(thumbnail.isEmpty()) {
            return QString();
        }
        QString cachedFile;
        if(Kodi::instance()->imageCache()->contains(thumbnail, 1, cachedFile)) {
            return cachedFile;
        }
        // Size optimized for big representations.
        int job = Kodi::instance()->imageCache()->fetch(thumbnail, const_cast<KodiModel*>(this), "imageFetched", QSize(1000, 1000), 1);
        m_imageFetchJobs.insert(job, index.row());
        return QString("loading");
    }
    if(role == RoleDuration) {
        QTime duration = m_list.at(index.row())->data(role).toTime();
        if(duration.hour() > 0) {
            return duration.toString("hh:mm:ss");
        }
        return duration.toString("mm:ss");
    }
    return m_list.at(index.row())->data(role);
}

int KodiModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex KodiModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex KodiModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int KodiModel::findItem(const QString &string, bool caseSensitive)
{
    foreach(KodiModelItem* item, m_list) {
        if(caseSensitive) {
            if(item->data(RoleTitle).toString().startsWith(string)) {
                return m_list.indexOf(item);
            }
        } else {
            if(item->data(RoleTitle).toString().toLower().startsWith(string.toLower())) {
                return m_list.indexOf(item);
            }
        }
    }
    return -1;
}

QHash<int, QByteArray> KodiModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleTitle, "title");
    roleNames.insert(RoleFileType, "filetype");
    roleNames.insert(RoleSubtitle, "subtitle");
    roleNames.insert(RoleDuration, "duration");
    roleNames.insert(RolePlayable, "playable");
    roleNames.insert(RoleFileName, "filename");
    roleNames.insert(RoleThumbnail, "thumbnail");
    roleNames.insert(RoleLargeThumbnail, "largeThumbnail");
    roleNames.insert(RoleSortingTitle, "sortingTitle");
    roleNames.insert(RolePlot, "plot");
    roleNames.insert(RoleRating, "rating");
    roleNames.insert(RoleSeason, "season");
    roleNames.insert(RoleEpisode, "episode");
    roleNames.insert(RoleFirstAired, "firstaired");
    roleNames.insert(RoleGenre, "genre");
    roleNames.insert(RoleYear, "year");
    roleNames.insert(RoleDirector, "director");
    roleNames.insert(RoleTagline, "tagline");
    roleNames.insert(RoleMpaa, "mpaa");
    roleNames.insert(RoleDescription, "description");
    roleNames.insert(RoleInstrument, "instrument");
    roleNames.insert(RoleStyle, "style");
    roleNames.insert(RoleMood, "mood");
    roleNames.insert(RoleBorn, "born");
    roleNames.insert(RoleFormed, "formed");
    roleNames.insert(RoleDied, "died");
    roleNames.insert(RoleDisbanded, "disbanded");
    roleNames.insert(RoleDuration, "duration");
    roleNames.insert(RoleComment, "comment");
    roleNames.insert(RolePlaycount, "playcount");
    roleNames.insert(RoleCast, "cast");
    roleNames.insert(RolePlayingState, "playingState");
    roleNames.insert(RoleLockMode, "lockMode");
    roleNames.insert(RoleResume, "resume");
    roleNames.insert(RoleResumeString, "resumeString");
    return roleNames;
}

bool KodiModel::busy() const
{
    return m_busy;
}

void KodiModel::setBusy(bool busy)
{
    m_busy = busy;
    emit busyChanged();
}

bool KodiModel::ignoreArticle() const
{
    return m_ignoreArticle;
}

void KodiModel::setIgnoreArticle(bool ignoreArticle)
{
    m_ignoreArticle = ignoreArticle;
    emit ignoreArticleChanged();
}

void KodiModel::imageFetched(int id)
{
    if(m_imageFetchJobs.contains(id)) {
        QModelIndex changedIndex = index(m_imageFetchJobs.value(id), 0, QModelIndex());
        emit dataChanged(changedIndex, changedIndex);
        m_imageFetchJobs.remove(id);
    }
}
