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

int XbmcHostModel::insertOrUpdateHost(const XbmcHost &newHost)
{
    for(int i = 0; i < m_hosts.count(); ++i) {
        if(m_hosts.at(i)->address() == newHost.address()) {
            XbmcHost *oldHost = m_hosts.at(i);
            oldHost->setHostname(newHost.hostname());
            oldHost->setPort(newHost.port());
            if(!newHost.hwAddr().isEmpty()) {
                oldHost->setHwAddr(newHost.hwAddr());
            }
            if(!newHost.username().isEmpty()) {
                oldHost->setUsername(newHost.username());
            }
            if(!newHost.password().isEmpty()) {
                oldHost->setPassword(newHost.password());
            }
            oldHost->setXbmcHttpSupported(newHost.xbmcHttpSupported());
            oldHost->setXbmcJsonrpcSupported(newHost.xbmcJsonrpcSupported());
            emit dataChanged(index(i), index(i));
            qDebug() << "host updated";
            return i;
        }
    }
    beginInsertRows(QModelIndex(), m_hosts.count(), m_hosts.count());
    XbmcHost *host = new XbmcHost();
    host->setAddress(newHost.address());
    host->setHostname(newHost.hostname());
    host->setHwAddr(newHost.hwAddr());
    host->setPort(newHost.port());
    host->setUsername(newHost.username());
    host->setPassword(newHost.password());
    host->setXbmcHttpSupported(newHost.xbmcHttpSupported());
    host->setXbmcJsonrpcSupported(newHost.xbmcJsonrpcSupported());
    host->setVolumeUpCommand(newHost.volumeUpCommand());
    host->setVolumeDownCommand(newHost.volumeDownCommand());
    m_hosts.append(host);
    endInsertRows();

    qDebug() << "host inserted";
    return m_hosts.count() - 1;
}

int XbmcHostModel::createHost(const QString &hostname, const QString &ip, int port, const QString &macAddress)
{
    XbmcHost host;
    host.setHostname(hostname);
    host.setAddress(ip);
    host.setHwAddr(macAddress);
    host.setPort(port);
    host.setXbmcHttpSupported(true);
    host.setXbmcJsonrpcSupported(true);
    return insertOrUpdateHost(host);
}

void XbmcHostModel::removeHost(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_hosts.removeAt(index);
    endRemoveRows();
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

/*******************************************************
  XbmcHost implementation
 *******************************************************/

XbmcHost::XbmcHost():
    m_xbmcJsonrpcSupported(false),
    m_xbmcHttpSupported(false),
    m_port(0)
{

}

QString XbmcHost::hostname() const
{
    return m_hostname;
}

void XbmcHost::setHostname(const QString &hostname)
{
    m_hostname = hostname;
}

QString XbmcHost::address() const
{
    return m_address;
}

void XbmcHost::setAddress(const QString &address)
{
    m_address = address;
}

QString XbmcHost::username() const
{
    return m_username;
}

void XbmcHost::setUsername(const QString &username)
{
    m_username = username;
}

QString XbmcHost::password() const
{
    return m_password;
}

void XbmcHost::setPassword(const QString &password)
{
    m_password = password;
}

QString XbmcHost::volumeUpCommand() const
{
    return m_volumeUpCommand;
}

void XbmcHost::setVolumeUpCommand(const QString &command)
{
    m_volumeUpCommand = command;
}

QString XbmcHost::volumeDownCommand() const
{
    return m_volumeDownCommand;
}

void XbmcHost::setVolumeDownCommand(const QString &command)
{
    m_volumeDownCommand = command;
}

bool XbmcHost::xbmcJsonrpcSupported() const
{
    return m_xbmcJsonrpcSupported;
}

void XbmcHost::setXbmcJsonrpcSupported(bool supported)
{
    m_xbmcJsonrpcSupported = supported;
}

bool XbmcHost::xbmcHttpSupported() const
{
    return m_xbmcHttpSupported;
}

void XbmcHost::setXbmcHttpSupported(bool supported)
{
    m_xbmcHttpSupported = supported;
}

QString XbmcHost::hwAddr() const
{
    return m_hwAddr;
}

void XbmcHost::setHwAddr(const QString &hwaddr)
{
    m_hwAddr = hwaddr;
}

int XbmcHost::port() const
{
    return m_port;
}

void XbmcHost::setPort(int port)
{
    m_port = port;
}
