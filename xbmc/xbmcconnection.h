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

#ifndef XBMCCONNECTION_H
#define XBMCCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkAccessManager>

class XbmcHost;
class XbmcDownload;

namespace XbmcConnection
{

void connect(XbmcHost *host);
bool connecting();
XbmcHost *connectedHost();
void setAuthCredentials(const QString &username, const QString &password);

bool connected();
QString connectionError();
int xbmcVersion();

int sendCommand(const QString &command, const QVariant &params = QVariant());
void sendLegacyCommand(const QString &command);

QNetworkAccessManager *nam();

void download(XbmcDownload *download);

class Notifier: public QObject
{
    Q_OBJECT
public:
    friend class XbmcConnectionPrivate;
signals:
    void connectionChanged();
    void receivedAnnouncement(const QVariantMap &announcement);
    void responseReceived(int id, const QVariantMap &response);
    void authenticationRequired(const QString &hostname, const QString &address);
    void downloadAdded(XbmcDownload *download);
};
Notifier *notifier();
}

#endif // XBMCCONNECTION_H
