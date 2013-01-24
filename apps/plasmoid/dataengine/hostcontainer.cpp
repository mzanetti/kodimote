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

#include "hostcontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmcdiscovery.h"
#include "libxbmcremote/xbmchostmodel.h"

HostContainer::HostContainer(QObject *parent): Plasma::DataContainer(parent)
{
    m_discovery = new XbmcDiscovery(this);
    refresh();
    //discovery->setContinuousDiscovery(true);

    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(hostFound()));
}

void HostContainer::hostFound()
{
    if(Xbmc::instance()->hostModel()->rowCount(QModelIndex()) >= 1) {
        QStringList hostList;

        for(int i = 0; i < Xbmc::instance()->hostModel()->rowCount(QModelIndex()); ++i) {
            QModelIndex index = Xbmc::instance()->hostModel()->index(i, 0);
            qDebug() << "setting new data";
            hostList << Xbmc::instance()->hostModel()->data(index, Qt::DisplayRole).toString();
        }

	setData("hostList", hostList);
    }

    setNeedsUpdate(true);
    checkForUpdate();
}

void HostContainer::refresh()
{
    m_discovery->discover();
}
