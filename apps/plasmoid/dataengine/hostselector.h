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

#ifndef HOSTSELECTOR_H
#define HOSTSELECTOR_H

#include <Plasma/Service>

class HostContainer;

class HostSelector : public Plasma::Service
{
    Q_OBJECT

public:
    HostSelector(HostContainer *container, QObject* parent);

    Plasma::ServiceJob* createJob(const QString& operation, QMap<QString,QVariant>& parameters);

signals:
    void enabledOperationsChanged();

private slots:
    void updateEnabledOperations();

private:

    HostContainer *m_container;
};

#endif
