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

#include "xbmchostmodel.h"
#include "xbmcconnection.h"

#include <QSettings>

XbmcHostModel::XbmcHostModel(QObject *parent) :
    QAbstractListModel(parent)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif

    QSettings settings;
    settings.beginGroup("Hosts");
    QUuid lastConnectedId = settings.value("LastConnected").toUuid();
    foreach(const QString &hostGroup, settings.childGroups()) {
        XbmcHost *host = XbmcHost::fromSettings(hostGroup);
        addHost(host);
        if (host->id() == lastConnectedId) {
            host->connect();
        }
    }
    connect(XbmcConnection::notifier(), SIGNAL(connectionChanged()), this, SLOT(connectionChanged()));
}

int XbmcHostModel::addHost(XbmcHost *newHost)
{
    qDebug() << "added host" << newHost->id();
    newHost->setParent(this);
    beginInsertRows(QModelIndex(), m_hosts.count(), m_hosts.count());
    m_hosts.append(newHost);
    endInsertRows();

    qDebug() << "host inserted";
    emit countChanged();
    return m_hosts.count() - 1;
}

XbmcHost *XbmcHostModel::host(int index) const
{
    if (index < m_hosts.size()) {
        return m_hosts.at(index);
    } else {
        return NULL;
    }
}

void XbmcHostModel::removeHost(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    XbmcHost *host = m_hosts.takeAt(index);
    endRemoveRows();
    emit countChanged();

    QSettings settings;
    settings.beginGroup("Hosts");
    settings.beginGroup(host->id().toString());
    settings.remove("");

    host->deleteLater();
}

void XbmcHostModel::connectionChanged()
{
    if (!XbmcConnection::connected()) {
        return;
    }

    QSettings settings;
    settings.beginGroup("Hosts");
    settings.setValue("LastConnected", XbmcConnection::connectedHost()->id().toString());
}

QHash<int, QByteArray> XbmcHostModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleHostname, "hostname");
    roleNames.insert(RoleIP, "ip");
    roleNames.insert(RolePort, "port");
    roleNames.insert(RoleHwAddr, "hwaddr");
    return roleNames;
}

int XbmcHostModel::rowCount(const QModelIndex &) const
{
    return m_hosts.count();
}

int XbmcHostModel::count() const
{
    return m_hosts.count();
}

XbmcHost *XbmcHostModel::findHost(const QString &hwAddr) const
{
    for(int i = 0; i < m_hosts.count(); ++i) {
        if(m_hosts.at(i)->hwAddr() == hwAddr) {
            return m_hosts.at(i);
        }
    }
    return NULL;
}

QVariant XbmcHostModel::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case RoleHostname:
        return m_hosts.at(index.row())->hostname();
    case RoleIP:
        return m_hosts.at(index.row())->address();
    case RolePort:
        return m_hosts.at(index.row())->port();
    case RoleHwAddr:
        return m_hosts.at(index.row())->hwAddr();
    }
    return QVariant();
}

