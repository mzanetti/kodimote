// the following header is required by the LGPL because
// we are using the actual time engine code
/*
 *   Copyright 2007 Aaron Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
 
#include "xbmcremoteengine.h"
 
#include <QDate>
#include <QTime>
 
#include <KSystemTimeZones>
#include <KDateTime>
 
#include <Plasma/DataContainer>

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"
#include "libxbmcremote/xbmcdiscovery.h"

#include "hostcontainer.h"
#include "hostselector.h"
#include "xbmcservice.h"
#include "playerservice.h"
#include "playercontainer.h"


XbmcremoteEngine::XbmcremoteEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    // We ignore any arguments - data engines do not have much use for them
    Q_UNUSED(args)
 
    // This prevents applets from setting an unnecessarily high
    // update interval and using too much CPU.
    // In the case of a clock that only has second precision,
    // a third of a second should be more than enough.
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
    // We do not have any special code to execute the
    // first time a source is requested, so we just call
    // updateSourceEvent().    
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


// This does the magic that allows Plasma to load
// this plugin.  The first argument must match
// the X-Plasma-EngineName in the .desktop file.
// The second argument is the name of the class in
// your plugin that derives from Plasma::DataEngine
K_EXPORT_PLASMA_DATAENGINE(xbmcremote, XbmcremoteEngine)
 
// this is needed since TestTimeEngine is a QObject
//#include "xbmcremoteengine.moc"
