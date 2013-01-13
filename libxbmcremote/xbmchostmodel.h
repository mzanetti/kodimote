#ifndef XBMCHOSTMODEL_H
#define XBMCHOSTMODEL_H

#include <QAbstractListModel>
#include <QHostInfo>

class XbmcHost;

class XbmcHostModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role{
        RoleHostname = Qt::DisplayRole,
        RoleIP = Qt::UserRole,
        RolePort,
        RoleHwAddr
    };

    explicit XbmcHostModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QVariant get(int row, const QString &roleName);
    Q_INVOKABLE void connectToHost(int row);
    Q_INVOKABLE void wakeup(int row);

    // inserts or updates the host and returns its index
    int insertOrUpdateHost(const XbmcHost &host);

    Q_INVOKABLE int createHost(const QString &hostname, const QString &ip, int port, const QString &macAddress = QString());
    Q_INVOKABLE void removeHost(int index);

    QHash<int, QByteArray> roleNames() const;

private:
    QList<XbmcHost*> m_hosts;

};

class XbmcHost
{
public:
    XbmcHost();
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
};

#endif // XBMCHOSTMODEL_H
