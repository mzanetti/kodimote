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

/*******************************************************
  XbmcHost implementation
 *******************************************************/

XbmcHost::XbmcHost(QObject *parent):
    QObject(parent),
    m_xbmcJsonrpcSupported(false),
    m_xbmcHttpSupported(false),
    m_port(0),
    m_volumeControlType(VolumeControlTypeAbsolute),
    m_volumeStepping(5)
{

}

QString XbmcHost::hostname() const
{
    return m_hostname;
}

void XbmcHost::setHostname(const QString &hostname)
{
    m_hostname = hostname;
    emit hostnameChanged();
}

QString XbmcHost::address() const
{
    return m_address;
}

void XbmcHost::setAddress(const QString &address)
{
    m_address = address;
    emit addressChanged();
}

QString XbmcHost::username() const
{
    return m_username;
}

void XbmcHost::setUsername(const QString &username)
{
    m_username = username;
    emit usernameChanged();
}

QString XbmcHost::password() const
{
    return m_password;
}

void XbmcHost::setPassword(const QString &password)
{
    m_password = password;
    emit passwordChanged();
}

QString XbmcHost::volumeUpCommand() const
{
    return m_volumeUpCommand;
}

void XbmcHost::setVolumeUpCommand(const QString &command)
{
    m_volumeUpCommand = command;
    emit volumeUpCommandChanged();
}

QString XbmcHost::volumeDownCommand() const
{
    return m_volumeDownCommand;
}

void XbmcHost::setVolumeDownCommand(const QString &command)
{
    m_volumeDownCommand = command;
    emit volumeDownCommandChanged();
}

bool XbmcHost::xbmcJsonrpcSupported() const
{
    return m_xbmcJsonrpcSupported;
}

void XbmcHost::setXbmcJsonrpcSupported(bool supported)
{
    m_xbmcJsonrpcSupported = supported;
    emit xbmcJsonrpcSupportedChanged();
}

bool XbmcHost::xbmcHttpSupported() const
{
    return m_xbmcHttpSupported;
}

void XbmcHost::setXbmcHttpSupported(bool supported)
{
    m_xbmcHttpSupported = supported;
    emit xbmcHttpSupportedChanged();
}

QString XbmcHost::hwAddr() const
{
    return m_hwAddr;
}

void XbmcHost::setHwAddr(const QString &hwaddr)
{
    m_hwAddr = hwaddr;
    emit hwAddrChanged();
}

int XbmcHost::port() const
{
    return m_port;
}

void XbmcHost::setPort(int port)
{
    m_port = port;
    emit portChanged();
}

XbmcHost::VolumeControlType XbmcHost::volumeControlType() const
{
    return m_volumeControlType;
}

void XbmcHost::setVolumeControlType(const VolumeControlType type)
{
    m_volumeControlType = type;
    emit volumeControlTypeChanged();
}

int XbmcHost::volumeStepping() const
{
    return m_volumeStepping;
}

void XbmcHost::setVolumeStepping(const int stepping)
{
    m_volumeStepping = stepping;
    emit volumeSteppingChanged();
}
