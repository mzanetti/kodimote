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

#ifndef XDEBUG_H
#define XDEBUG_H

#include <QDebug>

#define XDAREA_GENERAL     0
#define XDAREA_CONNECTION  1
#define XDAREA_PLAYER      2
#define XDAREA_LIBRARY     3
#define XDAREA_FILES       4
#define XDAREA_PLAYLIST    5
#define XDAREA_NETWORKDATA 6
#define XDAREA_EVENTCLIENT 7

#define xDebug(a) XDebug::check(a)

class FakeDevice: public QIODevice
{
    Q_OBJECT
public:
    qint64 readData(char *, qint64) { return 0; }
    qint64 writeData(const char *, qint64 len) { return len; }
};

class XDebug
{
public:
    XDebug();

    static QDebug check(int area = XDAREA_GENERAL)
    { if(s_allowedAreas.contains(area)) return QDebug(QtDebugMsg); return QDebug(&s_fakeDevice); }

    static void addAllowedArea(int area);

private:
    static FakeDevice s_fakeDevice;
    static QList<int> s_allowedAreas;

};


#endif // XDEBUG_H
