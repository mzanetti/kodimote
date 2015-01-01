/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef GESTUREHELPER_H
#define GESTUREHELPER_H

#include <QObject>
#include <QStateMachine>

class GestureHelper : public QObject
{
    Q_OBJECT
    Q_ENUMS(AreaState)

    Q_PROPERTY(AreaState currentState READ currentState NOTIFY currentStateChanged)

    Q_PROPERTY(int startX READ startX WRITE setStartX NOTIFY startXChanged)
    Q_PROPERTY(int startY READ startY WRITE setStartY NOTIFY startYChanged)

    Q_PROPERTY(int realX READ realX WRITE setRealX NOTIFY realXChanged)
    Q_PROPERTY(int realY READ realY WRITE setRealY NOTIFY realYChanged)

    Q_PROPERTY(int restrictedX READ restrictedX NOTIFY realXChanged)
    Q_PROPERTY(int restrictedY READ restrictedY NOTIFY realYChanged)

    Q_PROPERTY(int depth READ depth NOTIFY depthChanged)

public:
    enum AreaState {
        AreaCenter,
        AreaTop,
        AreaLeft,
        AreaRight,
        AreaBottom
    };
    explicit GestureHelper(QObject *parent = 0);

    AreaState currentState();
    
    int startX();
    int startY();
    void setStartX(int x);
    void setStartY(int y);

    int realX();
    int realY();
    void setRealX(int x);
    void setRealY(int y);

    int restrictedX();
    int restrictedY();

    int depth();

public slots:
    void reset();

signals:
    void currentStateChanged();
    void startXChanged();
    void startYChanged();
    void realXChanged();
    void realYChanged();
    void depthChanged();
    
private:

    int m_startX;
    int m_startY;
    int m_realX;
    int m_realY;
    int m_limit;
    int m_innerLimit;

    AreaState m_areaState;

};

#endif // GESTUREHELPER_H
