/*****************************************************************************
 * Copyright: 2011-2015 Michael Zanetti <michael_zanetti@gmx.net>            *
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

#include "protocolmanager.h"
#include "kodebug.h"
#include "kodi.h"

#include <QUrlQuery>

#include "youtubeprotocolhandler.h"
#include "nativeprotocolhandler.h"

ProtocolManager::ProtocolManager(QObject *parent) :
    QObject(parent)
{
    NativeProtocolHandler::registerAll(this);
    registerProtocol(new YoutubeProtocolHandler(this));

    connect(Kodi::instance(), SIGNAL(connectedChanged(bool)), this, SLOT(connectedChanged(bool)));
}

void ProtocolManager::registerProtocol(ProtocolHandler *handler)
{
    m_handlers.insert(handler->scheme(), handler);
}

QList<ProtocolHandler*> ProtocolManager::list() const
{
    return m_handlers.values();
}

ProtocolHandler *ProtocolManager::get(const QString &scheme) const
{
    if (!m_handlers.contains(scheme)) {
        return NULL;
    }

    return m_handlers[scheme];
}

void ProtocolManager::execute(const QUrl &url)
{
    QUrl finalUrl = url;
    qDebug() << "initial url" << url;
    // FIXME: We want the youtube handler not only for "youtube://"
    // but also for "http[s]://*youtube.*/" uris. Let's rewrite those
    // to youtube:// uris for now.
    // Probably we should change the registering of handlers
    // to allow specifying more than just schemes.
    if (finalUrl.host().contains("youtube")) {
        finalUrl = "youtube:///video/" + QUrlQuery(url.query()).queryItemValue("v");
    }
    koDebug() << "Handling url:" << finalUrl << finalUrl.path() << finalUrl.path().split("/").count();

    if (!m_handlers.contains(finalUrl.scheme())) {
        return;
    }

    ProtocolHandler *handler = m_handlers[finalUrl.scheme()];

    if (Kodi::instance()->connected()) {
        QUrlQuery query(finalUrl);
        handler->execute(finalUrl, query.hasQueryItem("queue"));
        m_cache.clear();
    } else {
        m_cache = finalUrl; // We only store one uri, in case the user keeps on sending things without realizing he needs to connect.
    }
}

void ProtocolManager::connectedChanged(bool connected)
{
    if (connected && !m_cache.isEmpty()) {
        execute(m_cache);
    }
}
