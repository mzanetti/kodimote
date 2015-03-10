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

#include "mpriscontroller.h"

#include <QGuiApplication>
#include <QtDBus/QtDBus>

#include "mprisapplication.h"
#include "mprisplayer.h"

MprisController::MprisController(ProtocolManager *protocols, PlatformHelper *platform, QObject *parent) :
    QObject(parent)
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    bus.registerService("org.mpris.MediaPlayer2.kodimote");
    new MprisPlayer(protocols, this);
    new MprisApplication(protocols, platform, this);

    bus.registerObject("/org/mpris/MediaPlayer2", this);
}
