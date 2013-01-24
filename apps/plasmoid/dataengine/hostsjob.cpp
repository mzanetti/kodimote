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

#include "hostsjob.h"
#include "hostcontainer.h"

#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"

#include <QDebug>


HostsJob::HostsJob(HostContainer *container, const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent):
  Plasma::ServiceJob(operation, operation, parameters, parent),
  m_container(container)
{

}

void HostsJob::start()
{
    qDebug() << "Should start operation:" << operationName() << parameters();

    if(operationName() == "Discover") {
        m_container->refresh();
    } else if(operationName() == "Connect") {
        Xbmc::instance()->hostModel()->connectToHost(parameters().value("id").toInt());
        connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
    }
}


void HostsJob::connectedChanged(bool connected)
{
    disconnect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), this, SLOT(connectedChanged(bool)));
    setError(NoError);
    emitResult();
}
