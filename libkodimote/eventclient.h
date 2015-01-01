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

#ifndef EVENTCLIENT_H
#define EVENTCLIENT_H

#include "kodi/kodiclient.h"

#include <QObject>
#include <QChar>
#include <QTimer>

class KodiHost;

class EventClient : public QObject
{
    Q_OBJECT
public:
    explicit EventClient(QObject *parent = 0);
    
    void setApplicationThumbnail(const QString &thumbnail);
    void connectToHost(KodiHost *host);
    void disconnectFromHost();

public slots:
    void sendKeypress(const QString &buttonName);
    void sendIrPress(const QString &buttonName);

private slots:
    void releaseButton();

private:
    CAddress m_kodiHost;
    int m_socket;
    QTimer m_releaseTimer;
    QString m_thumbnail;
    int m_cleared;
};

#endif // EVENTCLIENT_H
