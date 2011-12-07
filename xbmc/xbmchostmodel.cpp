#include "xbmchostmodel.h"
#include "xbmcconnection.h"

#include <QUdpSocket>

XbmcHostModel::XbmcHostModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleHostname, "hostname");
    roleNames.insert(RoleIP, "ip");
    roleNames.insert(RolePort, "port");
    roleNames.insert(RoleHwAddr, "hwaddr");
    setRoleNames(roleNames);

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
    m_hosts.append(host);
    endInsertRows();

    qDebug() << "host inserted";
    return m_hosts.count() - 1;
}

int XbmcHostModel::createHost(const QString &hostname, const QString &ip, int port)
{
    XbmcHost host;
    host.setHostname(hostname);
    host.setAddress(ip);
    host.setPort(port);
    host.setXbmcHttpSupported(true);
    host.setXbmcJsonrpcSupported(true);
    return insertOrUpdateHost(host);
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
        packet.append(QByteArray::fromHex(host->hwAddr().remove(':').toAscii()));
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
