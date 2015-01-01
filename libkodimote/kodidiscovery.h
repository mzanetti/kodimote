/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
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

class KodiHost;

class KodiDiscovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool continuousDiscovery READ continuousDiscovery WRITE setContinuousDiscovery)

public:
    explicit KodiDiscovery(QObject *parent = 0);
    ~KodiDiscovery();

    bool continuousDiscovery();
    void setContinuousDiscovery(bool cd);

public slots:
    void discover();

private slots:
    void readDatagram();

private:
    enum RecordType {
        RecordTypeQuery,
        RecordTypeAnswer,
        RecordTypeAuthority,
        RecordTypeAdditional
    };

    bool setMulticastGroup(const QHostAddress &groupAddress, bool join);


    void parseDatagram(const QByteArray &datagram, int &index, RecordType recordType, KodiHost *host = 0);
    void parsePtrRecord(const QByteArray &datagram, int &index, RecordType recordType);
    void parseTxtRecord(const QByteArray &datagram, int &index, RecordType recordType);

    /// @returns The SRV Port
    int parseSrvRecord(const QByteArray &datagram, int &index, RecordType recordType);

    /// @returns The IP address
    QString parseARecord(const QByteArray &datagram, int &index, RecordType recordType);

    void parseAAAARecord(const QByteArray &datagram, int &index, RecordType recordType);

    /// skip a record, adjusting the index
    void skipRecord(const QByteArray &datagram, int &index, RecordType recordType);


    /// Parses a label, following references and adjusting the index
    QString parseLabel(const QByteArray &datagram, int &index);

    /// Reads a string with the given length, not adjusting the index
    QString readString(const QByteArray &datagram, int index, int count);

    /// Reads a number with the giben length, not adjusting the index
    int readNum(const QByteArray &datagram, int index, int count);

    QTimer m_continuousDiscoveryTimer;
    QUdpSocket *m_socket;
    QHostAddress m_multicastAddress;
};

#endif // XBMCDISCOVERY_H
