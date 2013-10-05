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

#include "xbmcdiscovery.h"
#include "xbmchostmodel.h"
#include "xbmc.h"
#include "xdebug.h"

#include <QNetworkInterface>
#include <QStringList>

#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>

// Zeroconf query for _xbmc-jsonrpc._tcp, _http._tcp and _workstation._tcp

char query_jsonrpc_http_workstations[] = {
    // DNS-SD Header
    0x00, 0x00, 0x00, 0x00, // Flags (Standard Query)
    0x00, 0x03,             // Questions
    0x00, 0x00,             // Answer records
    0x00, 0x00,             // Authority records
    0x00, 0x00,             // Additional records

    // Question for _xbmc-jsonrpc._tcp
    0x0d,                   // Length of following ttext ield
    0x5f, 0x78, 0x62, 0x6d, 0x63, 0x2d, 0x6a, 0x73, // _xbmc-js
    0x6f, 0x6e, 0x72, 0x70, 0x63, 0x04, 0x5f, 0x74, // onrpc._t
    0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, // cp.local
    0x00,                   // \0
    0x00, 0x0c,             // Type PTR
    0x00, 0x01,             // Class: IN

    // Question for _http._tcp
    0x05,                   // Length of text field
    0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74,  // _http._t
    0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c,  // cp.local
    0x00,                   // \0
    0x00, 0x0c,             // Type PTR
    0x00 ,0x01,             // Class IN

    // Question for _workstation._tcp
    0x0c,                   // Length of text field
    0x5f, 0x77, 0x6f, 0x72, 0x6b, 0x73, 0x74, 0x61,  // _worksta
    0x74, 0x69, 0x6f, 0x6e, 0x04, 0x5f, 0x74, 0x63,  // tion._tc
    0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c,        // p.local
    0x00,                   // \0
    0x00, 0x0c,             // Type PTR
    0x00, 0x01              // Class IN
};

XbmcDiscovery::XbmcDiscovery(QObject *parent) :
    QObject(parent)
{

    m_socket = new QUdpSocket(this);
    m_socket->bind(5353, QUdpSocket::ShareAddress);

    m_multicastAddress = QHostAddress("224.0.0.251");
    setMulticastGroup(m_multicastAddress, true);
    connect(m_socket, SIGNAL(readyRead()), SLOT(readDatagram()));

    discover();

    connect(&m_continuousDiscoveryTimer, SIGNAL(timeout()), SLOT(discover()));
}

XbmcDiscovery::~XbmcDiscovery()
{
    setMulticastGroup(m_multicastAddress, false);
    delete m_socket;
}

void XbmcDiscovery::discover()
{
    xDebug(XDAREA_DISCOVERY) << "Sending Zeroconf Query packet";
    m_socket->writeDatagram(QByteArray(query_jsonrpc_http_workstations, 93), m_multicastAddress, 5353);
}

bool XbmcDiscovery::continuousDiscovery()
{
    return m_continuousDiscoveryTimer.isActive();
}

void XbmcDiscovery::readDatagram()
{
    xDebug(XDAREA_DISCOVERY) << "***** multicast packet recieve *****";


    while (m_socket->hasPendingDatagrams()) {

        xDebug(XDAREA_DISCOVERY) << "*********************************************";
        xDebug(XDAREA_DISCOVERY) << "************** next datagram ****************";
        xDebug(XDAREA_DISCOVERY) << "*********************************************";

        int size = m_socket->pendingDatagramSize();
        char *data = (char*)malloc(size);
        m_socket->readDatagram(data, size);
        QByteArray datagram = QByteArray(data, size).toHex();
        free(data);

        xDebug(XDAREA_DISCOVERY) << "Datagram size:" << size;
        xDebug(XDAREA_DISCOVERY) << datagram;
        int index = 0;

        // ignore transaction id
        index += 4;

        // ignore flags
        index += 4;

        // read number of queries in this datagram
        int queryCount = readNum(datagram, index, 4);
        index += 4;
        if(queryCount > 0) {
            xDebug(XDAREA_DISCOVERY) << "Questions:" << queryCount;
        }

        // read number of answer records in this datagram
        int answerCount = readNum(datagram, index, 4);
        index += 4;
        xDebug(XDAREA_DISCOVERY) << "Ansert RRs:" << answerCount;
        if(answerCount == 0) {
            xDebug(XDAREA_DISCOVERY) << "No answers... Discarding packet...";
            continue;
        }

        // read number of authority records in this datagram
        int authorityCount = readNum(datagram, index, 4);
        index += 4;
        xDebug(XDAREA_DISCOVERY) << "Authority RRs:" << authorityCount;

        // read number of additional records in this datagram
        int additionalCount = readNum(datagram, index, 4);
        index += 4;
        xDebug(XDAREA_DISCOVERY) << "Additional RRs:" << additionalCount;

        for (int i = 0; i < queryCount; ++i) {
            parseDatagram(datagram, index, RecordTypeQuery);
        }

        XbmcHost host;

        for (int i = 0; i < answerCount; ++i) {
            parseDatagram(datagram, index, RecordTypeAnswer, &host);
        }

        xDebug(XDAREA_DISCOVERY) << "Found host:" << host.hostname() << "IP:" << host.address() << "Port:" << host.port() << "XBMC-JSONRPC:" << host.xbmcJsonrpcSupported() << "XBMC-Web-Server:" << host.xbmcHttpSupported() << "MAC:" << host.hwAddr();

        if(!host.address().isEmpty()
                && host.port() != 0
                && host.xbmcJsonrpcSupported()
                && host.xbmcHttpSupported()) {

            Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        }

    }
}

bool XbmcDiscovery::setMulticastGroup(const QHostAddress &groupAddress, bool join)
{
    QNetworkInterface interface;
    int sockOpt = 0;
    void *sockArg;
    int sockArgSize;

    ip_mreq mreq4;
#ifndef QT_NO_IPV6
    ipv6_mreq mreq6;

    if (groupAddress.protocol() == QAbstractSocket::IPv6Protocol) {
        if(join) {
            sockOpt = IPV6_JOIN_GROUP;
        } else {
            sockOpt = IPV6_LEAVE_GROUP;
        }
        sockArg = &mreq6;
        sockArgSize = sizeof(mreq6);
        memset(&mreq6, 0, sizeof(mreq6));
        Q_IPV6ADDR ip6 = groupAddress.toIPv6Address();
        memcpy(&mreq6.ipv6mr_multiaddr, &ip6, sizeof(ip6));
        mreq6.ipv6mr_interface = interface.index();
    } else
#endif
        if (groupAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            if(join) {
                sockOpt = IP_ADD_MEMBERSHIP;
            } else {
                sockOpt = IP_DROP_MEMBERSHIP;
            }
            sockArg = &mreq4;
            sockArgSize = sizeof(mreq4);
            memset(&mreq4, 0, sizeof(mreq4));
            mreq4.imr_multiaddr.s_addr = htonl(groupAddress.toIPv4Address());
//            qDebug() << "set address to:" << groupAddress.toIPv4Address();

            if (interface.isValid()) {
//                qDebug() << "interface is valid!";
                QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
                if (!addressEntries.isEmpty()) {
                    QHostAddress firstIP = addressEntries.first().ip();
                    mreq4.imr_interface.s_addr = htonl(firstIP.toIPv4Address());
                } else {
//                    qDebug() << "error getting addressentries: network unreachable";
                    return false;
                }
            } else {
                mreq4.imr_interface.s_addr = INADDR_ANY;
            }
        } else {
            // unreachable
//            qDebug() << "error network unreachable: Protocol unsupported";
            return false;
        }

    int res = setsockopt(m_socket->socketDescriptor(), IPPROTO_IP, sockOpt, sockArg, sockArgSize);
    if (res == -1) {
        switch (errno) {
        case ENOPROTOOPT:
//            qDebug() << "Error setting sockopt: Unsupported";
            break;
        default:
//            qDebug() << "Error setting sockopt: Unknown Socket";
            break;
        }
        return false;
    }
    return true;
}

void XbmcDiscovery::parseDatagram(const QByteArray &datagram, int &index, RecordType recordType, XbmcHost *host)
{
    xDebug(XDAREA_DISCOVERY) << "**** start new datagram:" << datagram[index] << datagram[index+1] << datagram[index+2] << datagram[index+3]
                 << datagram[index+4] << datagram[index+5] << datagram[index+6] << datagram[index+7];

    QString serviceName = parseLabel(datagram, index);
    xDebug(XDAREA_DISCOVERY) << "FQDN:" << serviceName;
    QString type = datagram.left(index + 4).right(4);
    index += 4;

    if (host && serviceName.contains("_xbmc-jsonrpc._tcp.local")) {
        host->setHostname(serviceName.remove("._xbmc-jsonrpc._tcp.local"));
        host->setXbmcJsonrpcSupported(true);
    }

    if (host && serviceName.contains("_workstation._tcp.local")) {
        QString workstationName = serviceName.remove("._workstation._tcp.local");
        if (workstationName.contains('[') && workstationName.contains(']')) {
            workstationName = workstationName.split(']').first().split('[').last();
            host->setHwAddr(workstationName);
        }
    }

    if (type == "0021") {
        int port = parseSrvRecord(datagram, index, recordType);
        if (host && serviceName.contains("_http._tcp.local")) {
            host->setXbmcHttpSupported(true);
            host->setPort(port);
        }
    } else if (type == "0001") {
        QString address = parseARecord(datagram, index, recordType);
        if (host) {
            host->setAddress(address);
        }
        /*
    } else if (type == "000c") {
        parsePtrRecord(datagram, index, recordType);
    } else if (type == "0010") {
        parseTxtRecord(datagram, index, recordType);
    } else if (type == "001c") {
        parseAAAARecord(datagram, index, recordType);
*/
    } else {
        skipRecord(datagram, index, recordType);
    }

}

QString XbmcDiscovery::parseLabel(const QByteArray &datagram, int &index)
{
    QString label;

    if (datagram[index] == 'c') {
        int newIndex = (readNum(datagram, index, 4) - 0xC000) * 2;
        index += 4;
        label.append(parseLabel(datagram, newIndex));
    } else if (readNum(datagram, index, 2) == 0) {
        index += 2;
        return label;
    } else {
        // skipping len field;
        int len = readNum(datagram, index, 2);
        index += 2;
        for (int i = 0; i < len*2; i+=2) {
            QChar character = QChar(QByteArray::fromHex(datagram.right(datagram.length() - (index+i)).left(2))[0]);
            if ((datagram[index+i] == '0' && datagram[index+i+1] <= 'f')) {
                character = QChar('.');
            }
            label.append(character);
            //qDebug() << "appending char" << character;
        }
        index += len*2;

        if (readString(datagram, index, 2) == "00") {
            index += 2;
            return label;
        }

        if (datagram.length() - index > 0) {
            label.append('.' + parseLabel(datagram, index));
        }
    }
    return label;
}

void XbmcDiscovery::parsePtrRecord(const QByteArray &datagram, int &index, RecordType recordType)
{
    // Class
    index += 4;

    if (recordType == RecordTypeAnswer) {
        // TTL
        index += 8;

        int len = readNum(datagram, index, 4);
        index += 4;
        qDebug() << "PTR:" << parseLabel(datagram, index);
    }
}


void XbmcDiscovery::parseTxtRecord(const QByteArray &datagram, int &index, XbmcDiscovery::RecordType recordType)
{
    // class
    index += 4;

    // TTL
    index += 8;

    int len = readNum(datagram, index, 4);
    index += 4;
    QByteArray trimmedDatagram = datagram.left(index + len*2);
    qDebug() << "TXT:" << parseLabel(trimmedDatagram, index);
}

int XbmcDiscovery::parseSrvRecord(const QByteArray &datagram, int &index, XbmcDiscovery::RecordType recordType)
{
    // class
    index += 4;

    // TTL
    index += 8;

    int len = readNum(datagram, index, 4);
    index += 4;

    // priority
    index += 4;

    // weight
    index += 4;

    // port
    int port = readNum(datagram, index, 4);
    index += 4;

    xDebug(XDAREA_DISCOVERY) << "SRV:" << "port" << port << parseLabel(datagram, index);

    return port;
}

QString XbmcDiscovery::parseARecord(const QByteArray &datagram, int &index, XbmcDiscovery::RecordType recordType)
{
    // class
    index += 4;

    // TTL
    index += 8;

    int len = readNum(datagram, index, 4);
    index += 4;

    int ip1 = readNum(datagram, index, 2);
    index += 2;
    int ip2 = readNum(datagram, index, 2);
    index += 2;
    int ip3 = readNum(datagram, index, 2);
    index += 2;
    int ip4 = readNum(datagram, index, 2);
    index += 2;

    QString address(QString::number(ip1) + "." + QString::number(ip2) + "." + QString::number(ip3) + "." + QString::number(ip4));
    xDebug(XDAREA_DISCOVERY) << "A:" << address;

    return address;
}

void XbmcDiscovery::parseAAAARecord(const QByteArray &datagram, int &index, XbmcDiscovery::RecordType recordType)
{
    // class
    index += 4;

    if (recordType == RecordTypeQuery) {
        return;
    }

    // TTL
    index += 8;

    int len = readNum(datagram, index, 4);
    index += 4;

    // ipv6 addr
    xDebug(XDAREA_DISCOVERY) << "AAAA:" << readString(datagram, index, len*2);
    index += len*2;

}

void XbmcDiscovery::skipRecord(const QByteArray &datagram, int &index, RecordType recordType)
{
    // class
    index += 4;

    if (recordType == RecordTypeAnswer) {
        // TTL
        index += 8;

        int len = readNum(datagram, index, 4);
        index += 4;
        index += len*2;
        xDebug(XDAREA_DISCOVERY) << "skipping record. len:" << len;
    }

}

QString XbmcDiscovery::readString(const QByteArray &datagram, int index, int count)
{
    return datagram.left(index + count).right(count);
}

int XbmcDiscovery::readNum(const QByteArray &datagram, int index, int count)
{
    return datagram.right(datagram.length() - index).left(count).toInt(0, 16);
}

void XbmcDiscovery::setContinuousDiscovery(bool cd)
{
    if(cd) {
        m_continuousDiscoveryTimer.start(3000);
    } else {
        m_continuousDiscoveryTimer.stop();
    }
}
