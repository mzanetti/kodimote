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

#include "kodifiltermodel.h"

KodiFilterModel::KodiFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_hideWatched(false),
    m_sortOrder(Qt::AscendingOrder)
{
    setSortRole(KodiModel::RoleTitle);
}

QAbstractItemModel *KodiFilterModel::model() const
{
    return sourceModel();
}

void KodiFilterModel::setModel(QObject *model)
{
    setSourceModel(static_cast<QAbstractItemModel*>(model));
    emit modelChanged();
    sort(m_sortOrder);
}

void KodiFilterModel::setFilter(const QString &filter)
{
    m_filterString = filter;
    setFilterFixedString(m_filterString);
    emit filterChanged();
}

QString KodiFilterModel::filter() const
{
    return m_filterString;
}

void KodiFilterModel::setHideWatched(bool hideWatched)
{
    if (m_hideWatched != hideWatched) {
        m_hideWatched = hideWatched;
        emit hideWatchedChanged();
        invalidateFilter();
    }
}

bool KodiFilterModel::hideWatched() const
{
    return m_hideWatched;
}

void KodiFilterModel::setSortOrder(Qt::SortOrder sortOrder)
{
    if (m_sortOrder != sortOrder) {
        sort(0, sortOrder);
        emit layoutChanged();
        m_sortOrder = sortOrder;
        emit sortOrderChanged();
    }
}

int KodiFilterModel::mapToSourceIndex(int i)
{
    if (i < 0) {
        return i;
    }
    if (i >= sourceModel()->rowCount()) {
        return 0;
    }
    return mapToSource(index(i, 0, QModelIndex())).row();
}

bool KodiFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
        return false;
    }
    if (m_hideWatched && sourceModel()->data(sourceModel()->index(source_row, 0), KodiModel::RolePlaycount).toInt() > 0) {
        return false;
    }
    return true;
}

bool KodiFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // We're keeping sorting from kodi, just invert it. Comapre source rows:
    return left.row() < right.row();
}

