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

#ifndef XBMCHOSTMODEL_H
#define XBMCHOSTMODEL_H

#include <QAbstractListModel>
#include <QHostInfo>

#include "xbmchost.h"

class XbmcHost;

class XbmcHostModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Role{
        RoleHostname = Qt::DisplayRole,
        RoleIP = Qt::UserRole,
        RolePort,
        RoleHwAddr
    };

    explicit XbmcHostModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int count() const;

    XbmcHost *findHost(const QString &hwAddr) const;

    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QVariant get(int row, const QString &roleName);
    Q_INVOKABLE void connectToHost(int row);
    Q_INVOKABLE void wakeup(int row);

    // inserts or updates the host and returns its index
    Q_INVOKABLE int insertOrUpdateHost(XbmcHost *host);
    Q_INVOKABLE int createHost(const QString &hostname, const QString &ip, int port, const QString &macAddress = QString());
    Q_INVOKABLE XbmcHost *getHost(int index) const;
    Q_INVOKABLE void removeHost(int index);

    QHash<int, QByteArray> roleNames() const;

signals:
    void countChanged();

private:
    QList<XbmcHost*> m_hosts;

};

#endif // XBMCHOSTMODEL_H
