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

#include "mprisapplication.h"

#include "kodi.h"
#include "kodihost.h"
#include "keys.h"

MprisApplication::MprisApplication(ProtocolManager *protocols, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    m_protocols(protocols)
{
}

bool MprisApplication::canQuit() const
{
    return true;
}

bool MprisApplication::canSetFullscreen() const
{
    return false;
}

bool MprisApplication::canRaise() const
{
    return false;
}

bool MprisApplication::hasTrackList() const
{
    return false;
}

QString MprisApplication::identity() const
{
    KodiHost *host = Kodi::instance()->connectedHost();
    if (host) {
        return tr("Kodi on %1").arg(host->hostname());
    } else {
        return "Kodimote";
    }
}

QString MprisApplication::desktopEntry() const
{
#ifdef SAILFISH
    return "harbour-kodimote";
#else
    return "kodimote";
#endif
}

QStringList MprisApplication::supportedUriSchemes() const
{
    QStringList schemes;

    foreach (ProtocolHandler *protocol, m_protocols->list()) {
        schemes.append(protocol->scheme());
    }

    return schemes;
}

QStringList MprisApplication::supportedMimeTypes() const
{
    QStringList mimeTypes;
    return mimeTypes;
}

bool MprisApplication::fullscreen() const
{
    return true;
}

void MprisApplication::setFullscreen(bool fullscreen)
{
    Q_UNUSED(fullscreen)
}

void MprisApplication::Quit()
{
    Kodi::instance()->quit();
}

void MprisApplication::Raise()
{
}
