#include "xbmchost.h"

XbmcHost::XbmcHost(QObject *parent):
    QObject(parent),
    m_xbmcJsonrpcSupported(false),
    m_xbmcHttpSupported(false),
    m_port(8080),
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
