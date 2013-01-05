/*
 * Copyright 2008  Alex Merry <alex.merry@kdemail.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "xbmcservice.h"
#include "xbmcjob.h"

XbmcService::XbmcService(XbmcContainer *container, QObject* parent)
    : Plasma::Service(parent),
    m_container(container)
{
    setObjectName("Xbmc" + QLatin1String(" controller"));
    setName("org.kde.xbmcremote.Xbmc");
    setDestination("Xbmc");
}

void XbmcService::containerDestroyed()
{
}

Plasma::ServiceJob* XbmcService::createJob(const QString& operation,
                                             QMap<QString,QVariant>& parameters)
{
    return new XbmcJob(operation, parameters, this);
}

//#include "xbmcservice.moc"

// vim: sw=4 sts=4 et tw=100
