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

#ifndef XBMCREMOTEENGINE_H
#define XBMCREMOTEENGINE_H

#include <Plasma/DataEngine>


class XbmcService;
class HostSelector;
class HostContainer;
class PlayerService;
class PlayerContainer;


class XbmcremoteEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    XbmcremoteEngine(QObject* parent, const QVariantList& args);

    Plasma::Service* serviceForSource(const QString& source);

    QStringList sources() const;

protected:
    bool sourceRequestEvent(const QString& name);
    bool updateSourceEvent(const QString& source);

private:
  XbmcService *m_xbmcService;
  HostSelector *m_hostSelector;
  HostContainer *m_hostContainer;
  PlayerService *m_playerService;
  PlayerContainer *m_playerContainer;

};

#endif
