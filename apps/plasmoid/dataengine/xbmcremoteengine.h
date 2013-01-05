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
 
// a standard include guard to prevent problems if the
// header is included multiple times
#ifndef TESTTIMEENGINE_H
#define TESTTIMEENGINE_H
 
// We need the DataEngine header, since we are inheriting it
#include <Plasma/DataEngine>
 
 
 class XbmcService;
 class HostSelector;
 class HostContainer;
 class PlayerService;
 class PlayerContainer;
 
/**
 * This engine provides the current date and time for a given
 * timezone.
 *
 * "Local" is a special source that is an alias for the current
 * timezone.
 */
class XbmcremoteEngine : public Plasma::DataEngine
{
    // required since Plasma::DataEngine inherits QObject
    Q_OBJECT
 
    public:
        // every engine needs a constructor with these arguments
        XbmcremoteEngine(QObject* parent, const QVariantList& args);
 
	Plasma::Service* serviceForSource(const QString& source);
	
	QStringList sources() const;
	
    protected:
        // this virtual function is called when a new source is requested
        bool sourceRequestEvent(const QString& name);
 
        // this virtual function is called when an automatic update
        // is triggered for an existing source (ie: when a valid update
        // interval is set when requesting a source)
        bool updateSourceEvent(const QString& source);
	
	
private:
  XbmcService *m_xbmcService;
  HostSelector *m_hostSelector;
  HostContainer *m_hostContainer;
  PlayerService *m_playerService;
  PlayerContainer *m_playerContainer;

};
 
#endif // TESTTIMEENGINE_H