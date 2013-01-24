/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "networkauthhandler.h"

#include "xbmc.h"
#include "xbmcconnection.h"
#include "xbmchostmodel.h"

#include <QDebug>

NetworkAuthHandler::NetworkAuthHandler(QObject *parent) :
    QObject(parent)
{
}

void NetworkAuthHandler::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "***************auth required!";
    Q_UNUSED(reply)
    if(XbmcConnection::connectedHost() && !XbmcConnection::connectedHost()->username().isEmpty() && !XbmcConnection::connectedHost()->password().isEmpty()) {
        authenticator->setUser(XbmcConnection::connectedHost()->username());
        authenticator->setPassword(XbmcConnection::connectedHost()->password());
    }
}
