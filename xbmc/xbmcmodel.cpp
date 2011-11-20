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

#include <QDebug>

XbmcModel::XbmcModel(XbmcModel *parent) :
    QAbstractItemModel(parent),
    m_parentModel(parent)
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
