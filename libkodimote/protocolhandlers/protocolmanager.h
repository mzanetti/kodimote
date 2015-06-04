/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include <QObject>
#include <QHash>

#include "protocolhandler.h"

class ProtocolManager : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolManager(QObject *parent = 0);

    void registerProtocol(ProtocolHandler *handler);

    QList<ProtocolHandler*> list() const;
    ProtocolHandler *get(const QString &scheme) const;

    inline void execute(const QString &uri) { execute(QUrl(uri)); }
    Q_INVOKABLE void execute(const QUrl &url);

private slots:
    void connectedChanged(bool connected);

private:
    QHash<QString, ProtocolHandler*> m_handlers;

    QUrl m_cache; // In case we need to wait for the user to connect first
};

#endif // PROTOCOLMANAGER_H
