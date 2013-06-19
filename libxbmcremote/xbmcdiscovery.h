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

#ifndef XBMCDISCOVERY_H
#define XBMCDISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class XbmcHost;

class XbmcDiscovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool continuousDiscovery READ continuousDiscovery WRITE setContinuousDiscovery)

public:
    explicit XbmcDiscovery(QObject *parent = 0);
    ~XbmcDiscovery();

    bool continuousDiscovery();
    void setContinuousDiscovery(bool cd);

public slots:
    void discover();

private slots:
    void readDatagram();

private:
    bool setMulticastGroup(const QHostAddress &groupAddress, bool join);

    QString extractName(const QByteArray data, int index);

    QTimer m_continuousDiscoveryTimer;
    QUdpSocket *m_socket;
    QHostAddress m_multicastAddress;
};

#endif // XBMCDISCOVERY_H
