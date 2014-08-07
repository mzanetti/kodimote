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

#include <QUdpSocket>

XbmcHostModel::XbmcHostModel(QObject *parent) :
    QAbstractListModel(parent)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif
}

int XbmcHostModel::insertOrUpdateHost(XbmcHost *newHost)
{
    newHost->setParent(this);

    int i = m_hosts.indexOf(newHost);
    if (i >= 0) {
        emit dataChanged(index(i), index(i));
        qDebug() << "host updated";
        return i;
    }

    beginInsertRows(QModelIndex(), m_hosts.count(), m_hosts.count());
    m_hosts.append(newHost);
    endInsertRows();

    qDebug() << "host inserted";
    emit countChanged();
    return m_hosts.count() - 1;
}

int XbmcHostModel::createHost(const QString &hostname, const QString &ip, int port, const QString &macAddress)
{
    XbmcHost *host = new XbmcHost(this);
    host->setHostname(hostname);
    host->setAddress(ip);
    host->setHwAddr(macAddress);
    host->setPort(port);
    host->setXbmcHttpSupported(true);
    host->setXbmcJsonrpcSupported(true);
    return insertOrUpdateHost(host);
}

XbmcHost *XbmcHostModel::getHost(int index) const
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
    m_hosts.at(index)->deleteLater();
    m_hosts.removeAt(index);
    endRemoveRows();
    emit countChanged();
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

QVariant XbmcHostModel::get(int row, const QString &roleName)
{
    return data(index(row), roleNames().key(roleName.toLatin1()));
}

void XbmcHostModel::connectToHost(int row) {
    XbmcConnection::connect(m_hosts.at(row));
}

void XbmcHostModel::wakeup(int row)
{
    XbmcHost *host = m_hosts.at(row);
    if(host->hwAddr().isEmpty()) {
        qDebug() << "don't know MAC address of host" << host->hostname();
        return;
    }
    const char header[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    QByteArray packet = QByteArray::fromRawData(header, sizeof(header));
    for(int i = 0; i < 16; ++i) {
        packet.append(QByteArray::fromHex(host->hwAddr().remove(':').toLocal8Bit()));
    }
    qDebug() << "created magic packet:" << packet.toHex();

    QUdpSocket udpSocket;
    udpSocket.writeDatagram(packet.data(), packet.size(), QHostAddress::Broadcast, 9);
}
