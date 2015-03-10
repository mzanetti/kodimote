/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef PLATFORMHELPER_H
#define PLATFORMHELPER_H

#include <QObject>

class Settings;

class PlatformHelper : public QObject
{
    Q_OBJECT
public:
    explicit PlatformHelper(Settings *settings, QObject *parent = 0);

    virtual bool canRaise() const { return false; }
    virtual void raise() {}

protected:
    Settings *m_settings;

protected slots:
    void callStarted(bool incoming, const QString &caller);
    void callEnded();

private:
    bool m_musicPaused;
    bool m_videoPaused;
};

#endif // PLATFORMHELPER_H
