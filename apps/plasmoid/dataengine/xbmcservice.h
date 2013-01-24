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

#ifndef XBMCSERVICE_H
#define XBMCSERVICE_H

#include "xbmccontainer.h"

#include <Plasma/Service>

class XbmcService : public Plasma::Service
{
    Q_OBJECT

public:
    XbmcService(XbmcContainer *container, QObject* parent);

    Plasma::ServiceJob* createJob(const QString& operation, QMap<QString,QVariant>& parameters);

signals:
    void enabledOperationsChanged();

private slots:
    void containerDestroyed();

private:
    XbmcContainer *m_container;

};

#endif
