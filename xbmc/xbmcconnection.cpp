#include "xbmcconnection.h"
#include "xbmcconnection_p.h"

#include "JsonQt/lib/VariantToJson.h"
#include "JsonQt/lib/JsonToVariant.h"
#include <QTime>

namespace Xbmc
{
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

    m_timeoutTimer.setInterval(1000);
    m_timeoutTimer.setSingleShot(true);
    QObject::connect(&m_timeoutTimer, SIGNAL(timeout()), SLOT(clearPending()));

}

void XbmcConnectionPrivate::connect(const QString &hostname, int port)
{
    qDebug() << "connecting...";
    m_socket->connectToHost(hostname, port);

    if (m_socket->waitForConnected(5000)) {
        qDebug() << "connection established.";
        QObject::connect(m_socket, SIGNAL(readyRead()), SLOT(readData()));
        sendCommand("JSONRPC.Ping");

        m_hostName = hostname;
        m_port = port;
    } else {
        QString QStringErrorString = m_socket->errorString();
        qDebug() << QStringErrorString;
    }

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
    if(m_currentPendingId >= 0) {
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

        QString finalCommand = JsonQt::VariantToJson::parse(map);
        qDebug() << "<<< sending:" << finalCommand;
        m_socket->write(finalCommand.toLocal8Bit());
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
    foreach(const QString lineData, data.split('\n')) {
        if(lineData.isEmpty()) {
            continue;
        }
        QVariantMap rsp;
        QTime t = QTime::currentTime();
        qDebug() << "starting parsing";
        try {
            rsp = JsonQt::JsonToVariant::parse(lineData).toMap();
        } catch (JsonQt::ParseException) {
            qDebug() << "data is" << lineData;
            qFatal("caught ParseException.");
            return;
        }
        qDebug() << "finished parsing after" << t.msecsTo(QTime::currentTime());

        qDebug() << ">>> Incoming:" << data;

        if(rsp.value("method").toString() == "Announcement") {
            qDebug() << ">>> received announcement" << rsp.value("params").toMap();
            emit m_notifier->receivedAnnouncement(rsp.value("params").toMap());
            return;
        }
        if(rsp.value("id").toInt() >= 0) {
    //        qDebug() << ">>> received response" << rsp.value("result");
            emit m_notifier->responseReceived(rsp.value("id").toInt(), rsp.value("result"));
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

}
}
