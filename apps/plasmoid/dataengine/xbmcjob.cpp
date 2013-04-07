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

#include "xbmcjob.h"
#include "libxbmcremote/xbmc.h"

#include <QDebug>

XbmcJob::XbmcJob(const QString& operation,
                    QMap<QString,QVariant>& parameters,
                    QObject* parent):
  Plasma::ServiceJob(operation, operation, parameters, parent)
{


}


void XbmcJob::start()
{
    qDebug() << "should start job:" << operationName() << parameters().value("level");
  
    if(operationName() == "SetVolume") {
      Xbmc::instance()->setVolume(parameters().value("level").toInt());
      connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
    }
}
