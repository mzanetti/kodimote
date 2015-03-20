#include "kodihost.h"
#include "kodiconnection.h"

#include <QUdpSocket>
#include <QSettings>

KodiHost::KodiHost(const QUuid &id, QObject *parent):
    QObject(parent),
    m_id(id),
    m_persistent(false),
    m_kodiJsonrpcSupported(false),
    m_kodiHttpSupported(false),
    m_port(8080),
    m_volumeControlType(VolumeControlTypeAbsolute),
    m_volumeStepping(5)
{
}

KodiHost *KodiHost::fromSettings(const QUuid &id)
{
    QSettings settings;
    settings.beginGroup("Hosts");
    settings.beginGroup(id.toString());

    qDebug() << "creating host with id" << id;
    KodiHost *host = new KodiHost(id);
    host->setAddress(settings.value("Address").toString());
    host->setHostname(settings.value("Hostname").toString());
    host->setHwAddr(settings.value("MAC").toString());
    host->setUsername(settings.value("Username").toString());
    host->setPassword(settings.value("Password").toString());
    host->setPort(settings.value("Port").toInt());
    host->setKodiHttpSupported(true);
    host->setKodiJsonrpcSupported(true);
    host->setVolumeUpCommand(settings.value("VolumeUpCommand").toString());
    host->setVolumeDownCommand(settings.value("VolumeDownCommand").toString());
    host->setVolumeControlType((KodiHost::VolumeControlType)settings.value("VolumeControlType", KodiHost::VolumeControlTypeAbsolute).toInt());
    host->setVolumeStepping(settings.value("VolumeStepping", 5).toInt());
    host->setPersistent(true);
    return host;
}

QUuid KodiHost::id() const
{
    return m_id;
}

bool KodiHost::persistent() const
{
    return m_persistent;
}

void KodiHost::setPersistent(bool persistent)
{
    m_persistent = persistent;
    syncToDisk();
}

QString KodiHost::hostname() const
{
    return m_hostname;
}

void KodiHost::setHostname(const QString &hostname)
{
    if (m_hostname != hostname) {
        m_hostname = hostname;
        emit hostnameChanged();
        syncToDisk();
    }
}

QString KodiHost::address() const
{
    return m_address;
}

void KodiHost::setAddress(const QString &address)
{
    if (address != m_address) {
        m_address = address;
        emit addressChanged();
        syncToDisk();
    }
}

QString KodiHost::username() const
{
    return m_username;
}

void KodiHost::setUsername(const QString &username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged();
        syncToDisk();
    }
}

QString KodiHost::password() const
{
    return m_password;
}

void KodiHost::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged();
        syncToDisk();
    }
}

QString KodiHost::volumeUpCommand() const
{
    return m_volumeUpCommand;
}

void KodiHost::setVolumeUpCommand(const QString &command)
{
    if (m_volumeUpCommand != command) {
        m_volumeUpCommand = command;
        emit volumeUpCommandChanged();
        syncToDisk();
    }
}

QString KodiHost::volumeDownCommand() const
{
    return m_volumeDownCommand;
}

void KodiHost::setVolumeDownCommand(const QString &command)
{
    if (m_volumeDownCommand != command) {
        m_volumeDownCommand = command;
        emit volumeDownCommandChanged();
        syncToDisk();
    }
}

bool KodiHost::kodiJsonrpcSupported() const
{
    return m_kodiJsonrpcSupported;
}

void KodiHost::setKodiJsonrpcSupported(bool supported)
{
    if (m_kodiJsonrpcSupported != supported) {
        m_kodiJsonrpcSupported = supported;
        emit kodiJsonrpcSupportedChanged();
        syncToDisk();
    }
}

bool KodiHost::kodiHttpSupported() const
{
    return m_kodiHttpSupported;
}

void KodiHost::setKodiHttpSupported(bool supported)
{
    if (m_kodiHttpSupported != supported) {
        m_kodiHttpSupported = supported;
        emit kodiHttpSupportedChanged();
        syncToDisk();
    }
}

QString KodiHost::hwAddr() const
{
    return m_hwAddr;
}

void KodiHost::setHwAddr(const QString &hwAddr)
{
    if (m_hwAddr != hwAddr) {
        m_hwAddr = hwAddr;
        emit hwAddrChanged();
        syncToDisk();
    }
}

int KodiHost::port() const
{
    return m_port;
}

void KodiHost::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
        syncToDisk();
    }
}

KodiHost::VolumeControlType KodiHost::volumeControlType() const
{
    return m_volumeControlType;
}

void KodiHost::setVolumeControlType(const VolumeControlType type)
{
    if (m_volumeControlType != type) {
        m_volumeControlType = type;
        emit volumeControlTypeChanged();
        syncToDisk();
    }
}

int KodiHost::volumeStepping() const
{
    return m_volumeStepping;
}

void KodiHost::setVolumeStepping(const int stepping)
{
    if (m_volumeStepping) {
        m_volumeStepping = stepping;
        emit volumeSteppingChanged();
        syncToDisk();
    }
}

void KodiHost::connect()
{
    qDebug() << "connecting host" << parent();
    KodiConnection::connect(this);
}

void KodiHost::wakeup()
{
    if(m_hwAddr.isEmpty()) {
        qDebug() << "don't know MAC address of host" << m_hostname;
        return;
    }
    const unsigned char header[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    QByteArray packet = QByteArray::fromRawData(reinterpret_cast<const char *>(header), sizeof(header));
    QString hwAddr = m_hwAddr;
    for(int i = 0; i < 16; ++i) {
        packet.append(QByteArray::fromHex(hwAddr.remove(':').toLocal8Bit()));
    }
    qDebug() << "created magic packet:" << packet.toHex();

    QUdpSocket udpSocket;
    udpSocket.writeDatagram(packet.data(), packet.size(), QHostAddress::Broadcast, 9);

}

void KodiHost::syncToDisk()
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
