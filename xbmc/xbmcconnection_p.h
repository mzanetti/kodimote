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

#ifndef XBMC_P_H
#define XBMC_P_H

#include "xbmcconnection.h"
//#include "player.h"

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QNetworkAccessManager>

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

    void replyReceived();
private:
    QTcpSocket *m_socket;
    int m_commandId;
    Notifier *m_notifier;

    QList<Command> m_commandQueue;
    int m_currentPendingId;
    QTimer m_timeoutTimer;

    void sendNextCommand();
    void sendNextCommand2();

    QString m_hostName;
    int m_port;

    QNetworkAccessManager *m_network;

};
Q_GLOBAL_STATIC(XbmcConnectionPrivate, instance)


}
#endif // XBMC_P_H
