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

#include "xbmcremoteengine.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"
#include "libxbmcremote/xbmcdiscovery.h"

#include "hostcontainer.h"
#include "hostselector.h"
#include "xbmcservice.h"
#include "playerservice.h"
#include "playercontainer.h"

#include <Plasma/DataContainer>

XbmcremoteEngine::XbmcremoteEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    Q_UNUSED(args)

    setMinimumPollingInterval(333);

    m_hostContainer = new HostContainer(this);
    m_hostContainer->setObjectName("Hosts");
    m_hostSelector = new HostSelector(m_hostContainer, this);
    addSource(m_hostContainer);

    XbmcContainer *xbmc = new XbmcContainer(this);
    xbmc->setObjectName("Xbmc");
    m_xbmcService = new XbmcService(xbmc, this);
    addSource(xbmc);

    m_playerService = new PlayerService(this);
    m_playerContainer = new PlayerContainer(this);
    m_playerContainer->setObjectName("Player");
    addSource(m_playerContainer);

}

QStringList XbmcremoteEngine::sources() const
{
  return QStringList() << "Hosts" << "Xbmc" << "Player";
}

bool XbmcremoteEngine::sourceRequestEvent(const QString &name)
{
    return updateSourceEvent(name);
}

Plasma::Service* XbmcremoteEngine::serviceForSource(const QString& source)
{
    qDebug() << "serviceForSource requested:" << source;
    if(source == "Xbmc") {
        return m_xbmcService;
    }

    if(source == "Hosts") {
        return m_hostSelector;
    }

    if(source == "Player") {
        return m_playerService;
    }
    return 0;
}

bool XbmcremoteEngine::updateSourceEvent(const QString &name)
{
    if(name == "Xbmc") {
      return true;
    } else if(name == "Hosts") {
       m_hostContainer->refresh();
       return true;
    } else if(name == "Player") {
	m_playerContainer->update();
    }

    return false;
}

K_EXPORT_PLASMA_DATAENGINE(xbmcremote, XbmcremoteEngine)
