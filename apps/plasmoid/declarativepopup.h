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

#ifndef DECLARATIVE_POPUP_H
#define DECLARATIVE_POPUP_H

#include <Plasma/DeclarativeWidget>
#include <Plasma/Applet>

class QDeclarativeContext;

class DeclarativePopup : public Plasma::DeclarativeWidget
{
    Q_OBJECT

public:
    explicit DeclarativePopup(QGraphicsWidget *parent = 0);



private Q_SLOTS:
    void qmlCreationFinished();

    void newStatus(Plasma::ItemStatus status);

private:
    QDeclarativeContext * m_rootContext;
};

#endif
