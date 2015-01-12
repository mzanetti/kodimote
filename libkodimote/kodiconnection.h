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

#ifndef XBMCCONNECTION_H
#define XBMCCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkAccessManager>

class KodiHost;
class KodiDownload;

namespace KodiConnection
{

void connect(KodiHost *host);
bool connecting();
KodiHost *connectedHost();
void disconnectFromHost();
void setAuthCredentials(const QString &username, const QString &password);

bool connected();
QString connectionError();

bool active();
void setActive(bool active);

int sendCommand(const QString &command, const QVariant &params = QVariant());
int sendCommand(const QString &command, const QVariant &params, QObject *callbackReceiver, const QString &callbackMember);

QNetworkAccessManager *nam();

void download(KodiDownload *download);

class Notifier: public QObject
{
    Q_OBJECT
public:
    friend class KodiConnectionPrivate;
signals:
    void connectionChanged();
    void receivedAnnouncement(const QVariantMap &announcement);
    void authenticationRequired(const QString &hostname, const QString &address);
    void downloadAdded(KodiDownload *download);
};
Notifier *notifier();
}

#endif // XBMCCONNECTION_H
