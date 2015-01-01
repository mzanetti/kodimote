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

#ifndef RUMBLEEFFECT_H
#define RUMBLEEFFECT_H

#include <QObject>
#include <QFeedbackHapticsEffect>
QTM_USE_NAMESPACE

class RumbleEffect : public QObject
{
    Q_OBJECT
public:
    explicit RumbleEffect(QObject *parent = 0);
    
public slots:
    void start(int count = 1);

private:
    QFeedbackHapticsEffect *m_effect;
    bool m_disabled;
};

#endif // RUMBLEEFFECT_H
