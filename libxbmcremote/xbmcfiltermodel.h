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

#ifndef XBMCFILTERMODEL_H
#define XBMCFILTERMODEL_H

#include "xbmcmodel.h"

#include <QSortFilterProxyModel>

class XbmcFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY caseSensitivityChanged)

public:
    explicit XbmcFilterModel(QObject *parent = 0);
    
    QAbstractItemModel *model();
    void setModel(QObject* model);

    void setFilter(const QString &filter);
    QString filter();

    void setCaseSensitive(bool cs);
    bool caseSensitive();

    Q_INVOKABLE int mapToSourceIndex(int index);

signals:
    void modelChanged();
    void filterChanged();
    void caseSensitivityChanged();

private:
    QString m_filterString;
    bool m_cs;
};

#endif // XBMCFILTERMODEL_H
