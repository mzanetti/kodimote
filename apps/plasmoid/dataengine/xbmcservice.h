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

#ifndef XBMCSERVICE_H
#define XBMCSERVICE_H

#include "xbmccontainer.h"

#include <Plasma/Service>

class XbmcService : public Plasma::Service
{
    Q_OBJECT

public:
    XbmcService(XbmcContainer *container, QObject* parent);

    Plasma::ServiceJob* createJob(const QString& operation,
                                  QMap<QString,QVariant>& parameters);

signals:
    void enabledOperationsChanged();

private slots:
    void containerDestroyed();

private:
  
  XbmcContainer *m_container;
  
};

#endif // PLAYERCONTROL_H
