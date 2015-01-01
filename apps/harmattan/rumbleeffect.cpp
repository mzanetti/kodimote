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

#include "rumbleeffect.h"
#include <QDebug>
#include <QApplication>

RumbleEffect::RumbleEffect(QObject *parent) :
    QObject(parent),
    m_effect(0),
    m_disabled(false)
{
    if(QApplication::arguments().contains("-r")) {
        m_disabled = true;
    }
}

void RumbleEffect::start(int count)
{
    if(m_disabled) {
        return;
    }
    // Do not load the hapticseffect as long as we don't need it. It'll kill the debug output!
    if(!m_effect) {
        qDebug() << "Creating Rumble effect. Unfortunately it has a bug destroying the debug output. If you want to have logs, don't use the keypad or use the command line argument \"-r\".";
        m_effect = new QFeedbackHapticsEffect(this);
        m_effect->setAttackIntensity(0);
        m_effect->setAttackTime(250);
        m_effect->setIntensity(1.0);
        m_effect->setFadeTime(250);
        m_effect->setFadeIntensity(0);
        m_effect->setPeriod(100);
    }
    m_effect->setDuration(100 * count);
    m_effect->start();
}
