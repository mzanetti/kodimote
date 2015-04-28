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

#include "protocolmanager.h"

#if defined(QT5_BUILD)
#include <QUrlQuery>
#elif defined(QT4_BUILD)
#include <QUrl>
#endif

#include "youtubeprotocolhandler.h"
#include "nativeprotocolhandler.h"

ProtocolManager::ProtocolManager(QObject *parent) :
    QObject(parent)
{
    NativeProtocolHandler::registerAll(this);
    registerProtocol(new YoutubeProtocolHandler(this));
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
    if (!m_handlers.contains(url.scheme())) {
        return;
    }

    ProtocolHandler *handler = m_handlers[url.scheme()];

#if defined(QT5_BUILD)
    QUrlQuery query(url);
#elif defined(QT4_BUILD)
    QUrl query = url;
#endif
    handler->execute(url, query.hasQueryItem("queue"));
}
