#ifndef XBMCHOST_H
#define XBMCHOST_H

#include <QObject>
#include <QUuid>

class KodiHost : public QObject
{
    Q_OBJECT
    Q_ENUMS(VolumeControlType)
    Q_PROPERTY(bool kodiJsonrpcSupported READ kodiJsonrpcSupported WRITE setKodiJsonrpcSupported NOTIFY kodiJsonrpcSupportedChanged)
    Q_PROPERTY(bool kodiHttpSupported READ kodiHttpSupported WRITE setKodiHttpSupported NOTIFY kodiJsonrpcSupportedChanged)
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

    explicit KodiHost(const QUuid &id = QUuid::createUuid(), QObject *parent = 0);
    static KodiHost* fromSettings(const QUuid &id);

    QUuid id() const;
    bool persistent() const;
    void setPersistent(bool persistent);

    bool kodiJsonrpcSupported() const;
    void setKodiJsonrpcSupported(bool supported);

    bool kodiHttpSupported() const;
    void setKodiHttpSupported(bool supported);

    QString hwAddr() const;
    void setHwAddr(const QString &hwAddr);

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

public slots:
    void connect();
    void wakeup();

signals:
    void kodiJsonrpcSupportedChanged();
    void kodiHttpSupportedChanged();
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
    void syncToDisk();

private:
    QUuid m_id;
    bool m_persistent;
    QString m_hostname;
    QString m_address;
    QString m_username;
    QString m_password;
    bool m_kodiJsonrpcSupported;
    bool m_kodiHttpSupported;
    QString m_hwAddr;
    int m_port;
    QString m_volumeUpCommand;
    QString m_volumeDownCommand;
    VolumeControlType m_volumeControlType;
    int m_volumeStepping;
};

#endif // XBMCHOST_H
