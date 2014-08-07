#ifndef XBMCHOST_H
#define XBMCHOST_H

#include <QObject>

class XbmcHost : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool xbmcJsonrpcSupported READ xbmcJsonrpcSupported WRITE setXbmcJsonrpcSupported NOTIFY xbmcJsonrpcSupportedChanged)
    Q_PROPERTY(bool xbmcHttpSupported READ xbmcHttpSupported WRITE setXbmcHttpSupported NOTIFY xbmcJsonrpcSupportedChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(QString hwAddr READ hwAddr WRITE setHwAddr NOTIFY hwAddrChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString volumeUpCommand READ volumeUpCommand WRITE setVolumeUpCommand NOTIFY volumeUpCommandChanged)
    Q_PROPERTY(QString volumeDownCommand READ volumeDownCommand WRITE setVolumeDownCommand NOTIFY volumeDownCommandChanged)
    Q_PROPERTY(VolumeControlType volumeControlType READ volumeControlType WRITE setVolumeControlType NOTIFY volumeControlTypeChanged)
    Q_PROPERTY(int volumeStepping READ volumeStepping WRITE setVolumeStepping NOTIFY volumeSteppingChanged)

public:
    enum VolumeControlType {
        VolumeControlTypeAbsolute,
        VolumeControlTypeRelative,
        VolumeControlTypeCustom
    };

    explicit XbmcHost(QObject *parent = 0);
    bool xbmcJsonrpcSupported() const;
    void setXbmcJsonrpcSupported(bool supported);

    bool xbmcHttpSupported() const;
    void setXbmcHttpSupported(bool supported);

    QString hwAddr() const;
    void setHwAddr(const QString &hwaddr);

    QString hostname() const;
    void setHostname(const QString &hostname);

    QString address() const;
    void setAddress(const QString &address);

    int port() const;
    void setPort(int port);

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

    QString volumeUpCommand() const;
    void setVolumeUpCommand(const QString &command);

    QString volumeDownCommand() const;
    void setVolumeDownCommand(const QString &command);

    VolumeControlType volumeControlType()  const;
    void setVolumeControlType(const VolumeControlType type);

    int volumeStepping() const;
    void setVolumeStepping(const int stepping);

signals:
    void xbmcJsonrpcSupportedChanged();
    void xbmcHttpSupportedChanged();
    void hwAddrChanged();
    void hostnameChanged();
    void addressChanged();
    void portChanged();
    void usernameChanged();
    void passwordChanged();
    void volumeUpCommandChanged();
    void volumeDownCommandChanged();
    void volumeControlTypeChanged();
    void volumeSteppingChanged();

private:
    QString m_hostname;
    QString m_address;
    QString m_username;
    QString m_password;
    bool m_xbmcJsonrpcSupported;
    bool m_xbmcHttpSupported;
    QString m_hwAddr;
    int m_port;
    QString m_volumeUpCommand;
    QString m_volumeDownCommand;
    VolumeControlType m_volumeControlType;
    int m_volumeStepping;
};

#endif // XBMCHOST_H
