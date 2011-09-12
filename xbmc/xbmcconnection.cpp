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

QString connectionError()
{
    return instance()->connectionError();
}

/*****************************************************************
  Private impl
  ***************************************************************/

XbmcConnectionPrivate::XbmcConnectionPrivate(QObject *parent) :
    QObject(parent),
    m_commandId(0),
    m_versionRequestId(-1),
    m_currentPendingId(-1),
    m_connected(false)
{
    m_socket = new QTcpSocket();
    m_notifier = new XbmcConnection::Notifier();

    QObject::connect(m_socket, SIGNAL(readyRead()), SLOT(readData()));
//    QObject::connect(m_socket, SIGNAL(connected()), m_notifier, SIGNAL(connectionChanged()));
    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError()));
    QObject::connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), SLOT(slotDisconnected()));

    m_timeoutTimer.setInterval(1000);
    m_timeoutTimer.setSingleShot(true);
    QObject::connect(&m_timeoutTimer, SIGNAL(timeout()), SLOT(clearPending()));

    m_network = new QNetworkAccessManager();
}

void XbmcConnectionPrivate::connect(const QString &hostname, int port)
{
    m_socket->disconnectFromHost();

    m_hostName = hostname;
    m_port = port;

    qDebug() << "connecting to" << hostname;
    // We connect to telnet on port 9090 for the announcements
    m_socket->connectToHost(hostname, 9090);

    m_connectionError = "Connecting to " + hostname + "...";
    emit m_notifier->connectionChanged();
}

void XbmcConnectionPrivate::slotConnected()
{
    qDebug() << "Connected to remote host. Asking for version...";

    m_versionRequestId = m_commandId++;
    Command cmd(m_versionRequestId, "JSONRPC.Version", QVariant());
    m_commandQueue.prepend(cmd);
    sendNextCommand2();
}

void XbmcConnectionPrivate::slotDisconnected()
{
    if(!m_connected) {
        qDebug() << "No connection yet, cannot disconnect.";
    }
    qDebug() << "Disconnected";
    m_connected = false;
    m_connectionError = "The connection has been disconnected";
    emit m_notifier->connectionChanged();
}

void XbmcConnectionPrivate::socketError()
{
    QString errorString = m_socket->errorString();
    qDebug() << "connection error:" << errorString;
    m_connectionError = "Connection failed: " + errorString;
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
        request.setUrl(QUrl("http://" + m_hostName + ":" + QString::number(m_port) + "/jsonrpc"));

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
        qDebug() << "sending command to" << request.url() << ":" << dataStr.toLocal8Bit();
#endif
        QNetworkReply * reply = m_network->post(request, data);
        QObject::connect(reply, SIGNAL(finished()), SLOT(replyReceived()));

        m_currentPendingId = command.id();
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::replyReceived()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QString commands = reply->readAll();
    if(reply->error() != QNetworkReply::NoError) {
        m_socket->disconnectFromHost();
        m_connectionError = "Connection failed: " + reply->errorString();
        emit m_notifier->connectionChanged();
    }

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

        if(rsp.value("id").toInt() == m_versionRequestId) {
            if(rsp.value("result").toMap().value("version").toInt() >= 3) {
                sendNextCommand2();
                m_connected = true;
                m_connectionError.clear();
            } else {
                qDebug() << "XBMC is too old!";
                m_socket->disconnectFromHost();
                m_connectionError = "Connection failed: This version of xbmc is too old. Please upgrade to a newer version (at least from 05. Sep. 2011) which can be downloaded from http://mirrors.xbmc.org/nightlies/.";
            }
            emit m_notifier->connectionChanged();
            return;
        }

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
    qDebug() << "timeouttimer hit for comman" << m_commandId;
    if(m_commandId == m_versionRequestId) {
        qDebug() << "cannot ask for remote version... ";
        m_connectionError = "Connection to " + m_hostName + " timed out...";
        emit m_notifier->connectionChanged();
        m_commandQueue.clear();
    }
    m_currentPendingId = -1;
    sendNextCommand2();
}

Notifier *XbmcConnectionPrivate::notifier()
{
    return m_notifier;
}

bool XbmcConnectionPrivate::connected()
{
    return m_connected;
}

QString XbmcConnectionPrivate::connectionError()
{
    return m_connectionError;
}

}
