#ifndef XBMC_P_H
#define XBMC_P_H

#include "xbmcconnection.h"
#include "player.h"

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

namespace Xbmc
{

namespace XbmcConnection
{

class Command
{
public:
    Command(int id, const QString &command, const QVariant &params):
        m_id(id), m_command(command), m_params(params) {}

    int id() {return m_id;}
    QString command() {return m_command;}
    QVariant params() {return m_params;}

private:

    int m_id;
    QString m_command;
    QVariant m_params;
};

class XbmcConnectionPrivate : public QObject
{
    Q_OBJECT
public:
    explicit XbmcConnectionPrivate(QObject *parent = 0);

    void connect(const QString &hostname, int port);

    int sendCommand(const QString &command, const QVariant &parms = QVariant());

    QString vfsPath();

    Notifier *notifier();

    bool connected();

private slots:
    void readData();
    void clearPending();
    void socketError();
    void slotConnected();

private:
    QTcpSocket *m_socket;
    int m_commandId;
    Notifier *m_notifier;

    QList<Command> m_commandQueue;
    int m_currentPendingId;
    QTimer m_timeoutTimer;

    void sendNextCommand();

    QString m_hostName;
    int m_port;

};
Q_GLOBAL_STATIC(XbmcConnectionPrivate, instance)


}
}
#endif // XBMC_P_H
