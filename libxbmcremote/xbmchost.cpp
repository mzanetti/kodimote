#include "xbmchost.h"
#include "xbmcconnection.h"

#include <QUdpSocket>
#include <QSettings>

XbmcHost::XbmcHost(const QUuid &id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_persistent(false),
    m_xbmcJsonrpcSupported(false),
    m_xbmcHttpSupported(false),
    m_port(8080),
    m_volumeControlType(VolumeControlTypeAbsolute),
    m_volumeStepping(5)
{
}

XbmcHost *XbmcHost::fromSettings(const QUuid &id)
{
    QSettings settings;
    settings.beginGroup("Hosts");
    settings.beginGroup(id.toString());

    qDebug() << "creating host with id" << id;
    XbmcHost *host = new XbmcHost(id);
    host->setAddress(settings.value("Address").toString());
    host->setHostname(settings.value("Hostname").toString());
    host->setHwAddr(settings.value("MAC").toString());
    host->setUsername(settings.value("Username").toString());
    host->setPassword(settings.value("Password").toString());
    host->setPort(settings.value("Port").toInt());
    host->setXbmcHttpSupported(true);
    host->setXbmcJsonrpcSupported(true);
    host->setVolumeUpCommand(settings.value("VolumeUpCommand").toString());
    host->setVolumeDownCommand(settings.value("VolumeDownCommand").toString());
    host->setVolumeControlType((XbmcHost::VolumeControlType)settings.value("VolumeControlType", XbmcHost::VolumeControlTypeAbsolute).toInt());
    host->setVolumeStepping(settings.value("VolumeStepping", 5).toInt());
    host->setPersistent(true);
    return host;
}

QUuid XbmcHost::id() const
{
    return m_id;
}

bool XbmcHost::persistent() const
{
    return m_persistent;
}

void XbmcHost::setPersistent(bool persistent)
{
    m_persistent = persistent;
    syncToDisk();
}

QString XbmcHost::hostname() const
{
    return m_hostname;
}

void XbmcHost::setHostname(const QString &hostname)
{
    if (m_hostname != hostname) {
        m_hostname = hostname;
        emit hostnameChanged();
        syncToDisk();
    }
}

QString XbmcHost::address() const
{
    return m_address;
}

void XbmcHost::setAddress(const QString &address)
{
    if (address != m_address) {
        m_address = address;
        emit addressChanged();
        syncToDisk();
    }
}

QString XbmcHost::username() const
{
    return m_username;
}

void XbmcHost::setUsername(const QString &username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged();
        syncToDisk();
    }
}

QString XbmcHost::password() const
{
    return m_password;
}

void XbmcHost::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged();
        syncToDisk();
    }
}

QString XbmcHost::volumeUpCommand() const
{
    return m_volumeUpCommand;
}

void XbmcHost::setVolumeUpCommand(const QString &command)
{
    if (m_volumeUpCommand != command) {
        m_volumeUpCommand = command;
        emit volumeUpCommandChanged();
        syncToDisk();
    }
}

QString XbmcHost::volumeDownCommand() const
{
    return m_volumeDownCommand;
}

void XbmcHost::setVolumeDownCommand(const QString &command)
{
    if (m_volumeDownCommand != command) {
        m_volumeDownCommand = command;
        emit volumeDownCommandChanged();
        syncToDisk();
    }
}

bool XbmcHost::xbmcJsonrpcSupported() const
{
    return m_xbmcJsonrpcSupported;
}

void XbmcHost::setXbmcJsonrpcSupported(bool supported)
{
    if (m_xbmcJsonrpcSupported != supported) {
        m_xbmcJsonrpcSupported = supported;
        emit xbmcJsonrpcSupportedChanged();
        syncToDisk();
    }
}

bool XbmcHost::xbmcHttpSupported() const
{
    return m_xbmcHttpSupported;
}

void XbmcHost::setXbmcHttpSupported(bool supported)
{
    if (m_xbmcHttpSupported != supported) {
        m_xbmcHttpSupported = supported;
        emit xbmcHttpSupportedChanged();
        syncToDisk();
    }
}

QString XbmcHost::hwAddr() const
{
    return m_hwAddr;
}

void XbmcHost::setHwAddr(const QString &hwAddr)
{
    if (m_hwAddr != hwAddr) {
        m_hwAddr = hwAddr;
        emit hwAddrChanged();
        syncToDisk();
    }
}

int XbmcHost::port() const
{
    return m_port;
}

void XbmcHost::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
        syncToDisk();
    }
}

XbmcHost::VolumeControlType XbmcHost::volumeControlType() const
{
    return m_volumeControlType;
}

void XbmcHost::setVolumeControlType(const VolumeControlType type)
{
    if (m_volumeControlType != type) {
        m_volumeControlType = type;
        emit volumeControlTypeChanged();
        syncToDisk();
    }
}

int XbmcHost::volumeStepping() const
{
    return m_volumeStepping;
}

void XbmcHost::setVolumeStepping(const int stepping)
{
    if (m_volumeStepping) {
        m_volumeStepping = stepping;
        emit volumeSteppingChanged();
        syncToDisk();
    }
}

void XbmcHost::connect()
{
    XbmcConnection::connect(this);
}

void XbmcHost::wakeup()
{
    if(m_hwAddr.isEmpty()) {
        qDebug() << "don't know MAC address of host" << m_hostname;
        return;
    }
    const char header[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    QByteArray packet = QByteArray::fromRawData(header, sizeof(header));
    for(int i = 0; i < 16; ++i) {
        packet.append(QByteArray::fromHex(m_hwAddr.remove(':').toLocal8Bit()));
    }
    qDebug() << "created magic packet:" << packet.toHex();

    QUdpSocket udpSocket;
    udpSocket.writeDatagram(packet.data(), packet.size(), QHostAddress::Broadcast, 9);

}

void XbmcHost::syncToDisk()
{
    if (!m_persistent) {
        return;
    }

    QSettings settings;
    settings.beginGroup("Hosts");
    settings.beginGroup(m_id.toString());
    settings.setValue("Address", m_address);
    settings.setValue("Hostname", m_hostname);
    settings.setValue("Username", m_username);
    settings.setValue("Password", m_password);
    settings.setValue("MAC", m_hwAddr);
    settings.setValue("Port", m_port);
    settings.setValue("VolumeUpCommand", m_volumeUpCommand);
    settings.setValue("VolumeDownCommand", m_volumeDownCommand);
    settings.setValue("VolumeControlType", m_volumeControlType);
    settings.setValue("VolumeStepping", m_volumeStepping);
}
