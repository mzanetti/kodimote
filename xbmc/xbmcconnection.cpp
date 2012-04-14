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

#include "xdebug.h"

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QTime>
#include <QStringList>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QHostInfo>

#define DEBUGJSON

namespace XbmcConnection
{
void connect(XbmcHost *host)
{
    instance()->connect(host);
}

XbmcHost *connectedHost()
{
    return instance()->connectedHost();
}

void setAuthCredentials(const QString &username, const QString &password)
{
    instance()->setAuthCredentials(username, password);
}

int sendCommand(const QString &command, const QVariant &params)
{
   return instance()->sendCommand(command, params);
}

void sendLegacyCommand(const QString &command)
{
   return instance()->sendLegacyCommand(command);
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

QNetworkAccessManager *nam()
{
    return instance()->nam();
}

int xbmcVersion()
{
    return instance()->xbmcVersion();
}

/*****************************************************************
  Private impl
  ***************************************************************/

XbmcConnectionPrivate::XbmcConnectionPrivate(QObject *parent) :
    QObject(parent),
    m_commandId(0),
    m_versionRequestId(-1),
    m_host(0),
    m_connected(false)
{
    m_socket = new QTcpSocket();
    m_notifier = new XbmcConnection::Notifier();

    QObject::connect(m_socket, SIGNAL(readyRead()), SLOT(readData()));
//    QObject::connect(m_socket, SIGNAL(connected()), m_notifier, SIGNAL(connectionChanged()));
    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError()));
    QObject::connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), SLOT(slotDisconnected()));

    m_timeoutTimer.setInterval(5000);
    m_timeoutTimer.setSingleShot(true);
    QObject::connect(&m_timeoutTimer, SIGNAL(timeout()), SLOT(clearPending()));

    m_network = new QNetworkAccessManager();
    QObject::connect(m_network, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));

    m_reconnectTimer.setInterval(5000);
    m_reconnectTimer.setSingleShot(true);
    QObject::connect(&m_reconnectTimer, SIGNAL(timeout()), SLOT(connect()));
}

void XbmcConnectionPrivate::connect(XbmcHost *host)
{
    qDebug() << "connecting";
    // Stop the reconnect timer in case someone else triggers the connect
    m_reconnectTimer.stop();

    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }

    if(m_socket->state() == QAbstractSocket::ConnectingState) {
        m_socket->abort();
    }

    if(host != 0) {
        m_host = host;
    }

    xDebug(XDAREA_CONNECTION) << "connecting to" << m_host->hostname() << m_host->address() << m_host->username() << m_host->password();
    // We connect to telnet on port 9090 for the announcements
    m_socket->connectToHost(m_host->address(), 9090);

    m_connectionError = tr("Connecting to %1...").arg(m_host->hostname());
    emit m_notifier->connectionChanged();
}

XbmcHost* XbmcConnectionPrivate::connectedHost()
{
    return m_host;
}

void XbmcConnectionPrivate::slotConnected()
{
    xDebug(XDAREA_CONNECTION) << "Connected to remote host. Asking for version...";

    m_versionRequestId = m_commandId++;
    Command cmd(m_versionRequestId, "JSONRPC.Version");
    m_commandQueue.prepend(cmd);
    sendNextCommand2();
}

void XbmcConnectionPrivate::slotDisconnected()
{
    if(!m_connected) {
        xDebug(XDAREA_CONNECTION) << "No connection yet, cannot disconnect.";
    }
    xDebug(XDAREA_CONNECTION) << "Disconnected";
    m_connected = false;
    m_connectionError = tr("The connection has been disconnected");
    emit m_notifier->connectionChanged();
}

void XbmcConnectionPrivate::socketError()
{
    QString errorString = m_socket->errorString();
    xDebug(XDAREA_CONNECTION) << "socket error:" << errorString;

    if(m_socket->state() != QAbstractSocket::ConnectedState) {
        m_connectionError = tr("Connection failed: %1").arg(errorString);
        emit m_notifier->connectionChanged();
        // silently try to reconnect if the connection failed
        m_reconnectTimer.start();
    }
}

void XbmcConnectionPrivate::sendNextCommand2() {

    if(m_currentPendingCommand.id() >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
        xDebug(XDAREA_CONNECTION) << "cannot send... busy";
        return;
    }
    if(m_commandQueue.count() > 0) {
        Command command = m_commandQueue.takeFirst();

        QNetworkRequest request;
        request.setUrl(QUrl("http://" + m_host->address() + ":" + QString::number(m_host->port()) + "/jsonrpc"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QVariantMap map;
        map.insert("jsonrpc", "2.0");
        map.insert("method", command.command());
        map.insert("id", command.id());

        if(!command.params().isNull()) {
            map.insert("params", command.params());
        }

        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(map);
//        xDebug(XDAREA_CONNECTION) << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        xDebug(XDAREA_CONNECTION) << "sending command 1" << dataStr;
        xDebug(XDAREA_CONNECTION) << "sending command to" << request.url() << ":" << dataStr.toLocal8Bit();
#endif
        QNetworkReply * reply = m_network->post(request, data);
        QObject::connect(reply, SIGNAL(finished()), SLOT(replyReceived()));

        m_currentPendingCommand = Command(command.id(), command.command(), command.params(), data);
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::replyReceived()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QString commands = reply->readAll();
    reply->deleteLater();

    if(reply->error() != QNetworkReply::NoError) {
        m_socket->disconnectFromHost();
        m_connectionError = tr("Connection failed: %1").arg(reply->errorString());
        emit m_notifier->connectionChanged();
    }

    xDebug(XDAREA_CONNECTION) << "received reply:" << commands;

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
//        xDebug(XDAREA_CONNECTION) << "starting parsing";
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            xDebug(XDAREA_CONNECTION) << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
//        xDebug(XDAREA_CONNECTION) << "finished parsing after" << t.msecsTo(QTime::currentTime());

        xDebug(XDAREA_NETWORKDATA) << ">>> Incoming:" << rsp;

        if(rsp.value("id").toInt() == m_versionRequestId) {
            // If the remote xbmc has the version field not compiled in, assume its the latest known one
            xDebug(XDAREA_CONNECTION) << "Parsing XBMC version:" << rsp.value("result").toMap().value("version");
            m_xbmcVersion = rsp.value("result").toMap().value("version").toInt();
            if(m_xbmcVersion < 3) {
                qDebug() << "WARNING! XBMC is too old or version field not valid! Some features might not work";
                m_connected = false;
                m_connectionError = tr("Xbmcremote is designed to work with XBMC Eden (v11.0). It seems you have connected to an older version of XMBC. Please upgrade XBMC in order to use Xbmcremote.");
            } else {
                m_timeoutTimer.stop();
                m_currentPendingCommand = Command();
                sendNextCommand2();
                m_connected = true;
                m_connectionError.clear();
            }
            emit m_notifier->connectionChanged();
            continue;
        }

        if(rsp.value("params").toMap().value("sender").toString() == "xbmc") {
            xDebug(XDAREA_CONNECTION) << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
        }
        if(rsp.value("id").toInt() >= 0) {
            xDebug(XDAREA_CONNECTION) << ">>> received response" << rsp.value("result");

            if(rsp.contains("error")) {
                xDebug(XDAREA_GENERAL) << "Error reply received:";
                xDebug(XDAREA_GENERAL) << "Request:" <<  m_currentPendingCommand.raw();
                xDebug(XDAREA_GENERAL) << "Reply: " << lineData;
            }

            emit m_notifier->responseReceived(rsp.value("id").toInt(), rsp);
            int id = rsp.value("id").toInt();
            if(m_currentPendingCommand.id() == id) {
//                m_commandQueue.removeFirst();
                m_timeoutTimer.stop();
                m_currentPendingCommand = Command();
            }
            sendNextCommand2();
            continue;
        }
        xDebug(XDAREA_CONNECTION) << "received unhandled data" << commands;
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

void XbmcConnectionPrivate::sendLegacyCommand(const QString &command)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_host->address() + ':' + QString::number(m_host->port()) + "/xbmcCmds/xbmcHttp?command=" + command));
    qDebug() << "sending legacy command:" << request.url().toString();
    QNetworkReply *reply = m_network->get(request);
    QObject::connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void XbmcConnectionPrivate::sendNextCommand()
{
    if(m_currentPendingCommand.id() >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
//        xDebug(XDAREA_CONNECTION) << "cannot send... busy";
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
//        xDebug(XDAREA_CONNECTION) << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        xDebug(XDAREA_CONNECTION) << "sending command 1" << dataStr;
        xDebug(XDAREA_CONNECTION) << "sending command" << dataStr.toLocal8Bit();
#endif
        m_socket->write(data);
        m_currentPendingCommand = command;
        m_timeoutTimer.start();
    }
}

void XbmcConnectionPrivate::readData()
{
//    QString data = QString::fromUtf8(m_socket->readAll());
    QByteArray dataArray = m_socket->readAll();
    QString data(dataArray);
    xDebug(XDAREA_CONNECTION) << "<<<<<<<<<<<< Received:" << dataArray;
    m_socket->waitForReadyRead(10);
    while(!(data.endsWith("}") || data.endsWith("}\n") || data.isEmpty())) {
        xDebug(XDAREA_CONNECTION) << "***********************************";
        xDebug(XDAREA_CONNECTION) << data;
        xDebug(XDAREA_CONNECTION) << "data seems to be unfinished... rading more";
        m_socket->waitForReadyRead(100);
        QString tmp = m_socket->readAll();
        data.append(tmp);
        xDebug(XDAREA_CONNECTION) << tmp;
        xDebug(XDAREA_CONNECTION) << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><";
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
//        xDebug(XDAREA_CONNECTION) << "starting parsing";
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            xDebug(XDAREA_CONNECTION) << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
//        xDebug(XDAREA_CONNECTION) << "finished parsing after" << t.msecsTo(QTime::currentTime());

//        xDebug(XDAREA_CONNECTION) << ">>> Incoming:" << data;

        if(rsp.value("params").toMap().value("sender").toString() == "xbmc") {
            xDebug(XDAREA_CONNECTION) << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
        }
        if(rsp.value("id").toInt() >= 0) {
//            xDebug(XDAREA_CONNECTION) << ">>> received response" << rsp.value("result");
            emit m_notifier->responseReceived(rsp.value("id").toInt(), rsp);
            int id = rsp.value("id").toInt();
            if(m_currentPendingCommand.id() == id) {
//                m_commandQueue.removeFirst();
                m_timeoutTimer.stop();
                m_currentPendingCommand = Command();
            }
            sendNextCommand2();
            continue;
        }
        xDebug(XDAREA_CONNECTION) << "received unhandled data" << data;
    }
}

void XbmcConnectionPrivate::clearPending()
{
    xDebug(XDAREA_CONNECTION) << "timeouttimer hit for comman" << m_commandId << m_currentPendingCommand.id() << m_currentPendingCommand.command();
    if(m_commandId == m_versionRequestId) {
        xDebug(XDAREA_CONNECTION) << "cannot ask for remote version... ";
        m_connectionError = tr("Connection to %1 timed out...").arg(m_host->hostname());
        emit m_notifier->connectionChanged();
        m_commandQueue.clear();
    }
    m_currentPendingCommand = Command();
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

void XbmcConnectionPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    if(reply == m_lastAuthRequest) {
        m_connectionError = "Wrong username or password";
//        emit m_notifier->connectionChanged();
//        emit m_notifier->authenticationRequired(m_host->hostname(), m_host->address());
    }
    if(!m_host->username().isEmpty() && !m_host->password().isEmpty()) {
        authenticator->setUser(m_host->username());
        authenticator->setPassword(m_host->password());
        m_lastAuthRequest = reply;
    } else {
        emit m_notifier->authenticationRequired(m_host->hostname(), m_host->address());
    }
}

void XbmcConnectionPrivate::setAuthCredentials(const QString &username, const QString &password)
{
    qDebug() << "setting auth credentials";
    if(m_host) {
        m_host->setUsername(username);
        m_host->setPassword(password);
        connect(m_host);
    }
}

QNetworkAccessManager *XbmcConnectionPrivate::nam()
{
    return m_network;
}

int XbmcConnectionPrivate::xbmcVersion()
{
    return m_xbmcVersion;
}
}

