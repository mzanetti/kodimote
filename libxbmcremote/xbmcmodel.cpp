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

#include "xbmcmodel.h"
#include "xbmc.h"
#include "imagecache.h"
#include "player.h"

#include <QDebug>

XbmcModel::XbmcModel(XbmcModel *parent) :
    QAbstractItemModel(parent),
    m_parentModel(parent),
    m_busy(true),
    m_ignoreArticle(false)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif
}

XbmcModel::~XbmcModel()
{
    qDebug() << "deleting model";
    while(!m_list.isEmpty()) {
        m_list.takeFirst()->deleteLater();
    }
}

XbmcModel *XbmcModel::parentModel() const
{
    return m_parentModel;
}

QVariant XbmcModel::data(const QModelIndex &index, int role) const
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
        if(Xbmc::instance()->imageCache()->contains(thumbnail, 0)) {
            return Xbmc::instance()->imageCache()->cachedFile(thumbnail, 0);
        }
        // Size optimized for list view icons.
        int job = Xbmc::instance()->imageCache()->fetch(thumbnail, const_cast<XbmcModel*>(this), "imageFetched", QSize(152, 120), 0);
        m_imageFetchJobs.insert(job, index.row());
        return QString("loading");
    }
    if(role == RoleLargeThumbnail) {
        QString thumbnail = m_list.at(index.row())->data(RoleThumbnail).toString();
        if(thumbnail.isEmpty()) {
            return QString();
        }
        if(Xbmc::instance()->imageCache()->contains(thumbnail, 1)) {
            return Xbmc::instance()->imageCache()->cachedFile(thumbnail, 1);
        }
        // Size optimized for big representations.
        int job = Xbmc::instance()->imageCache()->fetch(thumbnail, const_cast<XbmcModel*>(this), "imageFetched", QSize(1000, 1000), 1);
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
    qDebug() << "got item" << m_list.count() << index.row();
    qDebug() << "got item" << m_list.at(index.row());
    qDebug() << "got item" << m_list.at(index.row())->data(role);
    return m_list.at(index.row())->data(role);
}

int XbmcModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex XbmcModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex XbmcModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int XbmcModel::findItem(const QString &string, bool caseSensitive)
{
    foreach(XbmcModelItem* item, m_list) {
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

QHash<int, QByteArray> XbmcModel::roleNames() const
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
    return roleNames;
}

bool XbmcModel::busy() const
{
    return m_busy;
}

void XbmcModel::setBusy(bool busy)
{
    m_busy = busy;
    emit busyChanged();
}

bool XbmcModel::ignoreArticle() const
{
    return m_ignoreArticle;
}

void XbmcModel::setIgnoreArticle(bool ignoreArticle)
{
    m_ignoreArticle = ignoreArticle;
    emit ignoreArticleChanged();
}

void XbmcModel::imageFetched(int id)
{
    if(m_imageFetchJobs.contains(id)) {
        QModelIndex changedIndex = index(m_imageFetchJobs.value(id), 0, QModelIndex());
        emit dataChanged(changedIndex, changedIndex);
        m_imageFetchJobs.remove(id);
    }
}
