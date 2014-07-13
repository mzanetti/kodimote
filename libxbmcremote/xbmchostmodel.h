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

#ifndef XBMCHOSTMODEL_H
#define XBMCHOSTMODEL_H

#include <QAbstractListModel>
#include <QHostInfo>

class XbmcHost;

class XbmcHostModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Role{
        RoleHostname = Qt::DisplayRole,
        RoleIP = Qt::UserRole,
        RolePort,
        RoleHwAddr
    };

    explicit XbmcHostModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int count() const;

    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QVariant get(int row, const QString &roleName);
    Q_INVOKABLE void connectToHost(int row);
    Q_INVOKABLE void wakeup(int row);

    // inserts or updates the host and returns its index
    int insertOrUpdateHost(const XbmcHost &host);

    Q_INVOKABLE int createHost(const QString &hostname, const QString &ip, int port, const QString &macAddress = QString());
    Q_INVOKABLE void removeHost(int index);

    QHash<int, QByteArray> roleNames() const;

signals:
    void countChanged();

private:
    QList<XbmcHost*> m_hosts;

};

class XbmcHost
{
public:
    enum VolumeControlType {
        VolumeControlTypeAbsolute,
        VolumeControlTypeRelative,
        VolumeControlTypeCustom
    };

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

    VolumeControlType volumeControlType()  const;
    void setVolumeControlType(const VolumeControlType type);

    int volumeStepping() const;
    void setVolumeStepping(const int stepping);

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

#endif // XBMCHOSTMODEL_H
