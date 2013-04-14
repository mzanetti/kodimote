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

#include "xbmcfiltermodel.h"

XbmcFilterModel::XbmcFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

QAbstractItemModel *XbmcFilterModel::model()
{
    return sourceModel();
}

void XbmcFilterModel::setModel(QObject *model)
{
    setSourceModel(static_cast<QAbstractItemModel*>(model));
    emit modelChanged();
}

void XbmcFilterModel::setFilter(const QString &filter)
{
    m_filterString = filter;
    setFilterFixedString(m_filterString);
    emit filterChanged();
}

QString XbmcFilterModel::filter()
{
    return m_filterString;
}

void XbmcFilterModel::setCaseSensitive(bool cs)
{
    setFilterCaseSensitivity(m_cs ? Qt::CaseSensitive : Qt::CaseInsensitive);
    emit caseSensitivityChanged();
}

bool XbmcFilterModel::caseSensitive()
{
    return filterCaseSensitivity() == Qt::CaseSensitive ? true : false;
}

int XbmcFilterModel::mapToSourceIndex(int i)
{
    if (i < 0) {
        return i;
    }
    if (i >= sourceModel()->rowCount()) {
        return 0;
    }
    return mapToSource(index(i, 0, QModelIndex())).row();
}

