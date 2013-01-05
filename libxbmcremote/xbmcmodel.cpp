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
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleTitle, "title");
    roleNames.insert(RoleFileType, "filetype");
    roleNames.insert(RoleSubtitle, "subtitle");
    roleNames.insert(RoleDuration, "duration");
    roleNames.insert(RolePlayable, "playable");
    roleNames.insert(RoleFileName, "filename");
    roleNames.insert(RoleThumbnail, "thumbnail");
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

    setRoleNames(roleNames);
}

XbmcModel::~XbmcModel()
{
    while(!m_list.isEmpty()) {
        delete m_list.takeFirst();
    }
}

XbmcModel *XbmcModel::parentModel() const
{
    return m_parentModel;
}

QVariant XbmcModel::data(const QModelIndex &index, int role) const
{
    // Lets add a cache here
    if(role == RoleThumbnail) {
        QString thumbnail = m_list.at(index.row())->data(role).toString();
        if(thumbnail.isEmpty()) {
            return QString();
        }
        if(Xbmc::instance()->imageCache()->contains(thumbnail)) {
            return Xbmc::instance()->imageCache()->cachedFile(thumbnail);
        }
        int job = Xbmc::instance()->imageCache()->fetch(thumbnail, const_cast<XbmcModel*>(this), "imageFetched");
        m_imageFetchJobs.insert(job, index.row());
        return QString();
    }
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
