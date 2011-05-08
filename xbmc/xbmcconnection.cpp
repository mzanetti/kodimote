/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "xbmcconnection.h"
#include "xbmcconnection_p.h"

#ifdef Q_WS_MAEMO_5
#include <parser.h>
#include <serializer.h>
#else
#include <qjson/parser.h>
#include <qjson/serializer.h>
#endif

#include <QTime>
#include <QStringList>

#define DEBUGJSON

namespace XbmcConnection
{
void connect(const QString &hostname, int port)
{
    instance()->connect(hostname, port);
}

int sendCommand(const QString &command, const QVariant &params)
{
   return instance()->sendCommand(command, params);
}

QString vfsPath()
{
    return instance()->vfsPath();
}

Notifier *notifier()
{
    return instance()->notifier();
}

bool connected()
{
    return instance()->connected();
}

/*****************************************************************
  Private impl
  ***************************************************************/

XbmcConnectionPrivate::XbmcConnectionPrivate(QObject *parent) :
    QObject(parent),
    m_commandId(0),
    m_currentPendingId(-1)
{
    m_socket = new QTcpSocket();
    m_notifier = new XbmcConnection::Notifier();

    QObject::connect(m_socket, SIGNAL(readyRead()), SLOT(readData()));
    QObject::connect(m_socket, SIGNAL(connected()), m_notifier, SIGNAL(connectionChanged()));
    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError()));
    QObject::connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), m_notifier, SIGNAL(connectionChanged()));

    m_timeoutTimer.setInterval(1000);
    m_timeoutTimer.setSingleShot(true);
    QObject::connect(&m_timeoutTimer, SIGNAL(timeout()), SLOT(clearPending()));

}

void XbmcConnectionPrivate::connect(const QString &hostname, int port)
{
    if(connected()) {
        m_socket->disconnectFromHost();
    }

    m_hostName = hostname;
    m_port = port;

    qDebug() << "connecting to" << hostname;
    m_socket->connectToHost(hostname, port);

}

void XbmcConnectionPrivate::slotConnected()
{
    qDebug() << "connected";
    emit m_notifier->connectionChanged();
    sendNextCommand();
}

void XbmcConnectionPrivate::socketError()
{
    qDebug() << "connection error";
    QString QStringErrorString = m_socket->errorString();
    qDebug() << QStringErrorString;
    emit m_notifier->connectionChanged();
}

QString XbmcConnectionPrivate::vfsPath()
{
    qDebug() << "returning vfs:" << "http://" + m_hostName + ":8080/vfs/";
    return "http://" + m_hostName + ":8080/vfs/";
}

int XbmcConnectionPrivate::sendCommand(const QString &command, const QVariant &params)
{
    int id = m_commandId++;
    Command cmd(id, command, params);
    m_commandQueue.append(cmd);
    sendNextCommand();

    if(m_commandId < 0) {
        m_commandId = 0;
    }
    return id;
}

void XbmcConnectionPrivate::sendNextCommand()
{
    if(m_currentPendingId >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
//        qDebug() << "cannot send... busy";
        return;
    }
    if(m_commandQueue.count() > 0) {
        Command command = m_commandQueue.takeFirst();

        QVariantMap map;
        map.insert("jsonrpc", "2.0");
        map.insert("method", command.command());
        map.insert("id", command.id());

        if(!command.params().isNull()) {
            map.insert("params", command.params());
        }

        QJson::Serializer serializer;
#ifdef DEBUGJSON
        qDebug() << "sending command" << serializer.serialize(map);
        m_socket->write(serializer.serialize(map));
#else
        bool ok = true;
        serializer.serialize(map, m_socket, &ok);
        if(!ok) {
            qDebug() << "Error sending command:" << serializer.serialize(map);
        }
#endif
        m_currentPendingId = command.id();
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::readData()
{
    QString data = m_socket->readAll();
    while(!data.endsWith("}\n") && !data.isEmpty()) {
//        qDebug() << "***********************************";
//        qDebug() << data;
//        qDebug() << "data seems to be unfinished... rading more";
        m_socket->waitForReadyRead(100);
        QString tmp = m_socket->readAll();
        data.append(tmp);
//        qDebug() << tmp;
//        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><";
    }
//    qDebug() << "<<<<<<<<<<<< Received:" << data;
    foreach(const QString lineData, data.split('\n')) {
        if(lineData.isEmpty()) {
            continue;
        }
        QVariantMap rsp;
//        QTime t = QTime::currentTime();
//        qDebug() << "starting parsing";
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toLocal8Bit(), &ok).toMap();
        if(!ok) {
            qDebug() << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
//        qDebug() << "finished parsing after" << t.msecsTo(QTime::currentTime());

        qDebug() << ">>> Incoming:" << data;

        if(rsp.value("params").toMap().value("sender").toString() == "xbmc") {
            qDebug() << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            return;
        }
        if(rsp.value("id").toInt() >= 0) {
//            qDebug() << ">>> received response" << rsp.value("result");
            emit m_notifier->responseReceived(rsp.value("id").toInt(), rsp);
            int id = rsp.value("id").toInt();
            if(m_currentPendingId == id) {
//                m_commandQueue.removeFirst();
                m_timeoutTimer.stop();
                m_currentPendingId = -1;
            }
            sendNextCommand();
            return;
        }
        qDebug() << "received unhandled data" << data;
    }
}

void XbmcConnectionPrivate::clearPending()
{
    int waitingFor = m_currentPendingId;
    readData();
    qDebug() << "timeouttimer hit!";
    if(m_currentPendingId != waitingFor) {
        qDebug() << "ok... data is here now..";
    } else {
        qDebug() << "Still nothing... discarding packet";
        m_currentPendingId = -1;
        sendNextCommand();
    }
}

Notifier *XbmcConnectionPrivate::notifier()
{
    return m_notifier;
}

bool XbmcConnectionPrivate::connected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

}
