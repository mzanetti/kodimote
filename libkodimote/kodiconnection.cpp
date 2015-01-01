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

#include "kodiconnection.h"
#include "kodiconnection_p.h"

#include "kodebug.h"
#include "kodidownload.h"

#ifdef QT5_BUILD
#include <QJsonDocument>
#else
#include <qjson/parser.h>
#include <qjson/serializer.h>
#endif

#include <QTime>
#include <QStringList>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QHostInfo>
#include <QDir>

#define DEBUGJSON

namespace KodiConnection
{
void connect(KodiHost *host)
{
    instance()->connect(host);
}

bool connecting()
{
    return instance()->connecting();
}

KodiHost *connectedHost()
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

int sendCommand(const QString &command, const QVariant &params, QObject *callbackReceiver, const QString &callbackMember)
{
    return instance()->sendCommand(command, params, callbackReceiver, callbackMember);
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

void disconnectFromHost()
{
    instance()->disconnectFromHost();
}

QNetworkAccessManager *nam()
{
    return instance()->nam();
}

void download(KodiDownload *download)
{
    instance()->download(download);
}

/*****************************************************************
  Private impl
  ***************************************************************/

KodiConnectionPrivate::KodiConnectionPrivate(QObject *parent) :
    QObject(parent),
    m_commandId(0),
    m_versionRequestId(-1),
    m_kodiVersionMajor(0),
    m_kodiVersionMinor(0),
    m_host(0),
    m_connecting(false),
    m_connected(false),
    m_networkSession(0)
{
    m_socket = new QTcpSocket();
    m_notifier = new KodiConnection::Notifier();

    m_connManager = new QNetworkConfigurationManager(this);
    QObject::connect(m_connManager, SIGNAL(onlineStateChanged(bool)), SLOT(connect()));

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

void KodiConnectionPrivate::connect(KodiHost *host)
{
    if(host != 0) {
        m_host = host;
    }
    if(m_host == 0) {
        qDebug() << "No host given and no previously connected host stored. Cannot connect.";
        return;
    }

    // Stop the reconnect timer in case someone else triggers the connect
    m_reconnectTimer.stop();

    // Don't automatically reconnect when device is offline and no host provided
    // In other words, prevent triggering "connect to network" dialog when the connect isn't user initiated
    if(!host && !m_connManager->isOnline()) {
        koDebug(XDAREA_CONNECTION) << "Device isn't online, we can't connect";
        return;
    }

    qDebug() << "connecting";
    m_connecting = true;

    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }

    if(m_socket->state() == QAbstractSocket::ConnectingState) {
        m_socket->abort();
    }

    if(m_networkSession) {
        QObject::disconnect(m_networkSession, SIGNAL(closed()), this, SLOT(sessionLost()));
        m_networkSession->close();
        m_networkSession->deleteLater();
    }

    QNetworkConfiguration networkConfig = m_connManager->defaultConfiguration();
    m_networkSession = new QNetworkSession(networkConfig, this);
    QObject::connect(m_networkSession, SIGNAL(closed()), this, SLOT(sessionLost()));
    m_networkSession->open();
#ifndef UBUNTU
    if(m_networkSession->isOpen()) {
        internalConnect();
    }
    else {
        qDebug() << "network session not open... not connecting...";
        QObject::connect(m_networkSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(internalConnect()));
    }
#else
    internalConnect();
#endif
}

void KodiConnectionPrivate::internalConnect()
{
#ifndef UBUNTU
    if(m_networkSession->state() != QNetworkSession::Connected) {
        qDebug() << m_networkSession->state();
        return;
    }
#endif
    if (!m_host) {
        return;
    }
    QObject::disconnect(m_networkSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(internalConnect()));
    QObject::connect(m_networkSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(sessionLost()));

    koDebug(XDAREA_CONNECTION) << "connecting to" << m_host->hostname() << m_host->address() << m_host->username() << "(using password:" << !m_host->password().isEmpty() << ")";
    // We connect to telnet on port 9090 for the announcements
    m_socket->connectToHost(m_host->address(), 9090);

    m_connectionError = tr("Connecting to %1...").arg(m_host->hostname());
    emit m_notifier->connectionChanged();
}

void KodiConnectionPrivate::sessionLost()
{
    if(m_networkSession->state() != QNetworkSession::Disconnected) {
        return;
    }

    if(m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
    else if(m_socket->state() == QAbstractSocket::ConnectingState) {
        m_socket->abort();
    }

    m_connected = false;
    notifier()->connectionChanged();

    m_networkSession->close();
    m_networkSession->deleteLater();
    m_networkSession = 0;
}

KodiHost* KodiConnectionPrivate::connectedHost()
{
    return m_host;
}

bool KodiConnectionPrivate::connecting()
{
    koDebug(XDAREA_CONNECTION) << "current socket state" << m_socket->state();
    return m_connecting;
}

void KodiConnectionPrivate::slotConnected()
{
    koDebug(XDAREA_CONNECTION) << "Connected to remote host. Asking for version...";

    m_versionRequestId = m_commandId++;
    Command cmd(m_versionRequestId, "JSONRPC.Version");
    m_commandQueue.prepend(cmd);
    sendNextCommand2();
}

void KodiConnectionPrivate::slotDisconnected()
{
    if(!m_connected) {
        koDebug(XDAREA_CONNECTION) << "No connection yet, cannot disconnect.";
    }
    koDebug(XDAREA_CONNECTION) << "Disconnected";
    m_connecting = false;
    m_connected = false;
    m_connectionError = tr("The connection has been disconnected");
    emit m_notifier->connectionChanged();
}

void KodiConnectionPrivate::socketError()
{
    QString errorString = m_socket->errorString();
    koDebug(XDAREA_CONNECTION) << "socket error:" << errorString << m_socket->error();

    if(m_socket->state() != QAbstractSocket::ConnectedState) {
        m_connectionError = tr("Connection failed: %1").arg(errorString);
        emit m_notifier->connectionChanged();
        // silently try to reconnect if the connection failed
        m_reconnectTimer.start();
    }
}

void KodiConnectionPrivate::sendNextCommand2() {

    if(m_currentPendingCommand.id() >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
        koDebug(XDAREA_CONNECTION) << "cannot send... busy";
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

#ifdef QT5_BUILD
        QJsonDocument jsonDoc = QJsonDocument::fromVariant(map);
        QByteArray data = jsonDoc.toJson();
#else
        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(map);
#endif

//        koDebug(XDAREA_CONNECTION) << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        koDebug(XDAREA_CONNECTION) << "sending command 1" << dataStr;
        koDebug(XDAREA_CONNECTION) << "sending command to" << request.url() << ":" << dataStr.toLocal8Bit();
#endif
        QNetworkReply * reply = m_network->post(request, data);
        QObject::connect(reply, SIGNAL(finished()), SLOT(replyReceived()));

        m_currentPendingCommand = Command(command.id(), command.command(), command.params(), data);
        m_timeoutTimer.start();
    }
}

void KodiConnectionPrivate::replyReceived()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender()); // We know its working... so don't waste time with typesafe casts
    reply->deleteLater();
    QString commands = reply->readAll();

    if(reply->error() != QNetworkReply::NoError) {
        m_socket->disconnectFromHost();
        m_connectionError = tr("Connection failed: %1").arg(reply->errorString());
        emit m_notifier->connectionChanged();
    }

    koDebug(XDAREA_CONNECTION) << "received reply:" << commands;

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

#ifdef QT5_BUILD
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lineData.toUtf8(), &error);

        if(error.error != QJsonParseError::NoError) {
            koDebug(XDAREA_CONNECTION) << "failed to parse data" << lineData << ":" << error.errorString();
            return;
        }
        rsp = jsonDoc.toVariant().toMap();
#else
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            koDebug(XDAREA_CONNECTION) << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
#endif

        koDebug(XDAREA_NETWORKDATA) << ">>> Incoming:" << rsp;

        if(rsp.value("id").toInt() == m_versionRequestId) {
            // If the remote kodi has the version field not compiled in, assume its the latest known one
            koDebug(XDAREA_CONNECTION) << "Parsing XBMC version:" << rsp.value("result").toMap().value("version");
            m_kodiVersionMajor = rsp.value("result").toMap().value("version").toMap().value("major").toInt();
            if(m_kodiVersionMajor < 6) {
                qDebug() << "WARNING! XBMC is too old or version field not valid! Some features might not work";
                m_connected = false;
                m_connectionError = tr("This version of Kodimote is designed to work with XBMC Frodo (v12.0). It seems you have connected to an older version of XMBC. Please upgrade XBMC in order to use Kodimote.");
            } else {
                m_timeoutTimer.stop();
                m_currentPendingCommand = Command();
                sendNextCommand2();
                m_connected = true;
                m_connectionError.clear();
            }
            m_connecting = false;
            emit m_notifier->connectionChanged();
            m_host->setPersistent(true);
            continue;
        }

        if(rsp.value("params").toMap().value("sender").toString() == "kodi") {
            koDebug(XDAREA_CONNECTION) << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
        }

        int id = rsp.value("id").toInt();
        if( id < 0) {
            koDebug(XDAREA_CONNECTION) << "received unhandled data" << commands;
            continue;
        }


        koDebug(XDAREA_CONNECTION) << ">>> received response" << rsp.value("result");

        if(rsp.contains("error")) {
            koDebug(XDAREA_GENERAL) << "Error reply received:";
            koDebug(XDAREA_GENERAL) << "Request:" <<  m_currentPendingCommand.raw();
            koDebug(XDAREA_GENERAL) << "Reply: " << lineData;
        }

        if(m_callbacks.contains(id)) {
            Callback callback = m_callbacks.take(id);
            if(!callback.receiver().isNull()) {
                QMetaObject::invokeMethod(callback.receiver().data(), callback.member().toLocal8Bit(), Qt::DirectConnection, Q_ARG(const QVariantMap&, rsp));
            }
        }

        if(m_currentPendingCommand.id() == id) {
//            m_commandQueue.removeFirst();
            m_timeoutTimer.stop();
            m_currentPendingCommand = Command();
        }
        sendNextCommand2();
        continue;
    }
}

int KodiConnectionPrivate::sendCommand(const QString &command, const QVariant &params)
{
    if(!(m_connected || m_connecting)) {
        qDebug() << "Not connected. Discarding command" << command;
        return -1;
    }

    int id = m_commandId++;
    Command cmd(id, command, params);
    m_commandQueue.append(cmd);
    sendNextCommand2();

    if(m_commandId < 0) {
        m_commandId = 0;
    }
    return id;
}

int KodiConnectionPrivate::sendCommand(const QString &command, const QVariant &params, QObject *callbackReceiver, const QString &callbackMember)
{
    if(!(m_connected || m_connecting)) {
        qDebug() << "Not connected. Discarding command" << command;
        return -1;
    }

    int id = m_commandId++;

    Callback callback(callbackReceiver, callbackMember);
    m_callbacks.insert(id, callback);

    Command cmd(id, command, params);
    m_commandQueue.append(cmd);
    sendNextCommand2();

    if(m_commandId < 0) {
        m_commandId = 0;
    }
    return id;
}

void KodiConnectionPrivate::sendLegacyCommand(const QString &command)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_host->address() + ':' + QString::number(m_host->port()) + "/kodiCmds/kodiHttp?command=" + command));
    qDebug() << "sending legacy command:" << request.url().toString();
    QNetworkReply *reply = m_network->get(request);
    QObject::connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void KodiConnectionPrivate::sendNextCommand()
{
    if(m_currentPendingCommand.id() >= 0 || m_socket->state() != QAbstractSocket::ConnectedState) {
//        koDebug(XDAREA_CONNECTION) << "cannot send... busy";
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

#ifdef QT5_BUILD
        QJsonDocument jsonDoc = QJsonDocument::fromVariant(map);
        QByteArray data = jsonDoc.toJson();
#else
        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(map);
#endif
//        koDebug(XDAREA_CONNECTION) << "ater serializing:" << data;
        QString dataStr = QString::fromLatin1(data);
#ifdef DEBUGJSON
//        koDebug(XDAREA_CONNECTION) << "sending command 1" << dataStr;
        koDebug(XDAREA_CONNECTION) << "sending command" << dataStr.toLocal8Bit();
#endif
        m_socket->write(data);
        m_currentPendingCommand = command;
        m_timeoutTimer.start();
    }
}

void KodiConnectionPrivate::readData()
{
//    QString data = QString::fromUtf8(m_socket->readAll());
    QByteArray dataArray = m_socket->readAll();
    QString data(dataArray);
    koDebug(XDAREA_CONNECTION) << "<<<<<<<<<<<< Received:" << dataArray;
    m_socket->waitForReadyRead(10);
    while(!(data.endsWith("}") || data.endsWith("}\n") || data.isEmpty())) {
        koDebug(XDAREA_CONNECTION) << "***********************************";
        koDebug(XDAREA_CONNECTION) << data;
        koDebug(XDAREA_CONNECTION) << "data seems to be unfinished... rading more";
        m_socket->waitForReadyRead(100);
        QString tmp = m_socket->readAll();
        data.append(tmp);
        koDebug(XDAREA_CONNECTION) << tmp;
        koDebug(XDAREA_CONNECTION) << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><";
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

#ifdef QT5_BUILD
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(lineData.toLocal8Bit(), &error);

        if(error.error != QJsonParseError::NoError) {
            koDebug(XDAREA_CONNECTION) << "failed to parse data" << lineData << ":" << error.errorString();
            return;
        }
        rsp = jsonDoc.toVariant().toMap();
#else
        QJson::Parser parser;
        bool ok;
        rsp = parser.parse(lineData.toAscii(), &ok).toMap();
        if(!ok) {
            koDebug(XDAREA_CONNECTION) << "data is" << lineData;
            qFatal("failed parsing.");
            return;
        }
#endif
        koDebug(XDAREA_CONNECTION) << ">>> Incoming:" << data;

        if(rsp.value("params").toMap().value("sender").toString() == "kodi") {
            koDebug(XDAREA_CONNECTION) << ">>> received announcement" << rsp;
            emit m_notifier->receivedAnnouncement(rsp);
            continue;
        }
        int id = rsp.value("id").toInt();
        if(m_callbacks.contains(id)) {
            Callback callback = m_callbacks.take(id);
            if(!callback.receiver().isNull()) {
                QMetaObject::invokeMethod(callback.receiver().data(), callback.member().toLocal8Bit(), Qt::DirectConnection, Q_ARG(const QVariantMap&, rsp));
            }

            if(m_currentPendingCommand.id() == id) {
//                m_commandQueue.removeFirst();
                m_timeoutTimer.stop();
                m_currentPendingCommand = Command();
            }
            sendNextCommand2();
            continue;
        }
        koDebug(XDAREA_CONNECTION) << "received unhandled data" << data;
    }
}

void KodiConnectionPrivate::clearPending()
{
    koDebug(XDAREA_CONNECTION) << "timeouttimer hit for comman" << m_commandId << m_currentPendingCommand.id() << m_currentPendingCommand.command();
    if(m_commandId == m_versionRequestId) {
        koDebug(XDAREA_CONNECTION) << "cannot ask for remote version... ";
        m_connectionError = tr("Connection to %1 timed out...").arg(m_host->hostname());
        emit m_notifier->connectionChanged();
        m_commandQueue.clear();
    }
    m_currentPendingCommand = Command();
    sendNextCommand2();
}

Notifier *KodiConnectionPrivate::notifier()
{
    return m_notifier;
}

void KodiConnectionPrivate::download(KodiDownload *download)
{
    qDebug() << "added download:" << download->source() << "-->" << download->destination();
    m_downloadQueue.append(download);
    emit m_notifier->downloadAdded(download);
    downloadNext();
}

void KodiConnectionPrivate::downloadNext()
{
    if(!m_activeDownloadsMap.isEmpty()) {
        qDebug() << "Download in progress... not starting next one";
        return;
    }
    if(m_downloadQueue.isEmpty()) {
        qDebug() << "Download queue empty";
        return;
    }
    KodiDownload *download = m_downloadQueue.takeFirst();
    qDebug() << "Starting download:" << download->source() << "-->" << download->destination();
    QByteArray url = QUrl::toPercentEncoding("http://" + KodiConnection::connectedHost()->address() + ':' + QString::number(KodiConnection::connectedHost()->port()) + '/');
    url.append(download->source().toUtf8());
    QNetworkRequest request;
    request.setUrl(QUrl::fromPercentEncoding(url));
    qDebug() << "getting:" << request.url();

    QFile *file = new QFile(download->destination());
    QFileInfo fi(download->destination());

    if(!fi.dir().exists()) {
        if(!fi.dir().mkpath(fi.dir().absolutePath())) {
            qDebug() << "cannot create dir" << fi.dir().absolutePath();
            download->setFinished(false);
            download->deleteLater();
            delete file;
            return;
        }
    }
    if(!file->open(QIODevice::ReadWrite)) {
        qDebug() << "cannot open destination" << download->destination();
        download->setFinished(false);
        download->deleteLater();
        delete file;
        return;
    }

    QNetworkReply *reply = m_network->get(request);
    QObject::connect(reply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
    QObject::connect(reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QObject::connect(download, SIGNAL(cancelled()), SLOT(cancelDownload()));
    qDebug() << reply->errorString();

    download->setFile(file);
    download->setStarted();
    m_activeDownloadsMap.insert(reply,download);
}

void KodiConnectionPrivate::downloadReadyRead()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    KodiDownload *download = m_activeDownloadsMap.value(reply);
    QFile *file = download->file();
    if(file->write(reply->readAll()) == -1) {
        reply->abort();
        reply->deleteLater();
        download->setFinished(false);
        download->deleteLater();
        file->remove(); // Try to delete the broken file
        delete file;
    }
}

void KodiConnectionPrivate::downloadProgress(qint64 progress, qint64 total)
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    KodiDownload *download = m_activeDownloadsMap.value(reply);
//    qDebug() << "updating progress:" << progress << "/" << total;
    download->setTotal(total);
    download->setProgress(progress);
}

void KodiConnectionPrivate::cancelDownload()
{
    KodiDownload *download = static_cast<KodiDownload*>(sender());
    QNetworkReply *reply = m_activeDownloadsMap.key(download);
    if(reply) {
        reply->close();
    }
}

void KodiConnectionPrivate::downloadFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    KodiDownload *download = m_activeDownloadsMap.value(reply);

    QFile *file = download->file();
    download->setFile(0);

    file->write(reply->readAll());
    file->close();
    delete file;

    if(reply->error() == QNetworkReply::NoError) {
        qDebug() << "download finished";
        download->setFinished(true);
    } else {
        qDebug() << "download failed" << reply->error() << reply->errorString();
        download->setFinished(false);
    }
    download->deleteLater();

    m_activeDownloadsMap.remove(reply);
    reply->deleteLater();

    downloadNext();
}

bool KodiConnectionPrivate::connected()
{
    return m_connected;
}

QString KodiConnectionPrivate::connectionError()
{
    return m_connectionError;
}

void KodiConnectionPrivate::disconnectFromHost()
{
    m_socket->disconnectFromHost();
    m_host = 0;
}

void KodiConnectionPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    koDebug(XDAREA_CONNECTION) << "Authentication required";
    if(reply == m_lastAuthRequest) {
        m_connectionError = "Wrong username or password";
        m_lastAuthRequest = 0;
        m_host->setPassword(QString());
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

void KodiConnectionPrivate::setAuthCredentials(const QString &username, const QString &password)
{
    qDebug() << "setting auth credentials";
    if(m_host) {
        m_host->setUsername(username);
        m_host->setPassword(password);
        connect(m_host);
    }
}

QNetworkAccessManager *KodiConnectionPrivate::nam()
{
    return m_network;
}

}

