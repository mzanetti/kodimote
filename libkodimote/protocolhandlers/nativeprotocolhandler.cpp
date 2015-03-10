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

#include "nativeprotocolhandler.h"

#include "../kodi.h"
#include "../videoplayer.h"

NativeProtocolHandler::NativeProtocolHandler(const QString &scheme, QObject *parent) :
    ProtocolHandler(parent),
    m_scheme(scheme)
{
}

QString NativeProtocolHandler::scheme() const
{
    return m_scheme;
}

void NativeProtocolHandler::execute(const QUrl &uri, bool queue)
{
    Q_UNUSED(queue)
    PlaylistItem item;
    item.setFile(uri.toString());

    //we don't know the type of medium, that's why we don't support adding to a playlist,
    //and can't use the audioPlayer as it always adds to the playlist and thus also depends on the type
    //TODO: investigate if we realy need different playlists, and can't just queue without playlistId
    Kodi::instance()->videoPlayer()->open(item);
}

void NativeProtocolHandler::registerAll(ProtocolManager *manager)
{
    manager->registerProtocol(new NativeProtocolHandler("http", manager));
    manager->registerProtocol(new NativeProtocolHandler("https", manager));
    manager->registerProtocol(new NativeProtocolHandler("smb", manager));
    manager->registerProtocol(new NativeProtocolHandler("sftp", manager));
    manager->registerProtocol(new NativeProtocolHandler("nfs", manager));
    manager->registerProtocol(new NativeProtocolHandler("upnp", manager));
    manager->registerProtocol(new NativeProtocolHandler("plugin", manager));
}
