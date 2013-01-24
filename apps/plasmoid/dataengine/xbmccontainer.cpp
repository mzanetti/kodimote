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

#include "xbmccontainer.h"

#include "libxbmcremote/xbmc.h"

XbmcContainer::XbmcContainer(QObject *parent): Plasma::DataContainer(parent)
{

  volumeChanged();
  connect(Xbmc::instance(), SIGNAL(volumeChanged(int)), SLOT(volumeChanged()));
  
  stateChanged();
  connect(Xbmc::instance(), SIGNAL(stateChanged()), SLOT(stateChanged()));
  
  connectedChanged(false);
  connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
}

void XbmcContainer::volumeChanged()
{
  qDebug() << "volume changed";
  setData("volume", Xbmc::instance()->volume());
  checkForUpdate();
}

void XbmcContainer::stateChanged()
{
    setData("state", Xbmc::instance()->state());
    checkForUpdate();
}

void XbmcContainer::connectedChanged(bool connected)
{
    setData("connected", connected);
    checkForUpdate();
}
