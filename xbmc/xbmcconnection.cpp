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

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QTime>
#include <QStringList>
#include <QNetworkRequest>
#include <QNetworkReply>

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
//    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError()));
    QObject::connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), m_notifier, SIGNAL(connectionChanged()));

    m_timeoutTimer.setInterval(1000);
    m_timeoutTimer.setSingleShot(true);
    QObject::connect(&m_timeoutTimer, SIGNAL(timeout()), SLOT(clearPending()));

    m_network = new QNetworkAccessManager();
}

void XbmcConnectionPrivate::connect(const QString &hostname, int port)
{
    if(connected()) {
        m_socket->disconnectFromHost();
    }

    m_hostName = hostname;
    m_port = port;

    qDebug() << "connecting to" << hostname;
    // We connect to telnet on port 9090 for the announcements
    m_socket->connectToHost(hostname, 9090);

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
    qDebug() << "returning vfs:" << "http://" + m_hostName + ':' + QString::number(m_port) + "/vfs/";
    return "http://" + m_hostName + ':' + QString::number(m_port) + "/vfs/";
}

void XbmcConnectionPrivate::sendNextCommand2() {

    if(m_currentPendingId >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
//        qDebug() << "cannot send... busy";
        return;
    }
    if(m_commandQueue.count() > 0) {
        Command command = m_commandQueue.takeFirst();

        QNetworkRequest request;
        request.setUrl(QUrl("http://10.10.10.100:8080/jsonrpc"));

        QVariantMap map;
        map.insert("jsonrpc", "2.0");
        map.insert("method", command.command());
        map.insert("id", command.id());

        if(!command.params().isNull()) {
            map.insert("params", command.params());
        }

        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(map);
//        qDebug() << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        qDebug() << "sending command 1" << dataStr;
        qDebug() << "sending command" << dataStr.toLocal8Bit();
#endif
        QNetworkReply * reply = m_network->post(request, data);
        QObject::connect(reply, SIGNAL(finished()), SLOT(replyReceived()));

        m_currentPendingId = command.id();
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::replyReceived()
{
    QString commands = static_cast<QNetworkReply*>(sender())->readAll();
    qDebug() << "received reply:" << commands;

    QStringList commandsList = commands.split("}{");

    for(int i = 0; i < commandsList.count(); ++i) {
        QString lineData = commandsList.at(i);
        if(lineData.isEmpty()) {
            continue;
        }
        // if we split at }{ the braces are removed... so lets add them again
        if(i < commandsList.count() - 1) {
            lineData.append("}");
        }
        if(i > 0) {
            lineData.prepend("{");
        }
        QVariantMap rsp;
//        QTime t = QTime::currentTime();
//        qDebug() << "starting parsing";
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            qDebug() << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
//        qDebug() << "finished parsing after" << t.msecsTo(QTime::currentTime());

//        qDebug() << ">>> Incoming:" << data;

        if(rsp.value("params").toMap().value("sender").toString() == "xbmc") {
            qDebug() << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
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
            sendNextCommand2();
            continue;
        }
        qDebug() << "received unhandled data" << commands;
    }
}

int XbmcConnectionPrivate::sendCommand(const QString &command, const QVariant &params)
{
    int id = m_commandId++;
    Command cmd(id, command, params);
    m_commandQueue.append(cmd);
    sendNextCommand2();

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
        QByteArray data = serializer.serialize(map);
//        qDebug() << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        qDebug() << "sending command 1" << dataStr;
        qDebug() << "sending command" << dataStr.toLocal8Bit();
#endif
        m_socket->write(data);
        m_currentPendingId = command.id();
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::readData()
{
//    QString data = QString::fromUtf8(m_socket->readAll());
    QByteArray dataArray = m_socket->readAll();
    QString data(dataArray);
    qDebug() << "<<<<<<<<<<<< Received:" << dataArray;
    m_socket->waitForReadyRead(10);
    while(!(data.endsWith("}") || data.endsWith("}\n") || data.isEmpty())) {
        qDebug() << "***********************************";
        qDebug() << data;
        qDebug() << "data seems to be unfinished... rading more";
        m_socket->waitForReadyRead(100);
        QString tmp = m_socket->readAll();
        data.append(tmp);
        qDebug() << tmp;
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><";
    }
    QStringList commandsList = data.split("}{");
    for(int i = 0; i < commandsList.count(); ++i) {
        QString lineData = commandsList.at(i);
        if(lineData.isEmpty()) {
            continue;
        }
        // if we split at }{ the braces are removed... so lets add them again
        if(i < commandsList.count() - 1) {
            lineData.append("}");
        }
        if(i > 0) {
            lineData.prepend("{");
        }
        QVariantMap rsp;
//        QTime t = QTime::currentTime();
//        qDebug() << "starting parsing";
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            qDebug() << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
//        qDebug() << "finished parsing after" << t.msecsTo(QTime::currentTime());

//        qDebug() << ">>> Incoming:" << data;

        if(rsp.value("params").toMap().value("sender").toString() == "xbmc") {
            qDebug() << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
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
            sendNextCommand2();
            continue;
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
        sendNextCommand2();
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
