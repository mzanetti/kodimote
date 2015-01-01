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

#ifndef XBMCFILTERMODEL_H
#define XBMCFILTERMODEL_H

#include "kodimodel.h"

#include <QSortFilterProxyModel>

class KodiFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool hideWatched READ hideWatched WRITE setHideWatched NOTIFY hideWatchedChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)

public:
    explicit KodiFilterModel(QObject *parent = 0);
    
    QAbstractItemModel *model() const;
    void setModel(QObject* model);

    void setFilter(const QString &filter);
    QString filter() const;

    void setHideWatched(bool hideWatched);
    bool hideWatched() const;

    void setSortOrder(Qt::SortOrder sortOrder);

    Q_INVOKABLE int mapToSourceIndex(int index);

signals:
    void modelChanged();
    void filterChanged();
    void hideWatchedChanged();
    void sortOrderChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString m_filterString;
    bool m_hideWatched;
    Qt::SortOrder m_sortOrder;
};

#endif // XBMCFILTERMODEL_H
