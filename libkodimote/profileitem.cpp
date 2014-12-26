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

#include "profileitem.h"

ProfileItem::ProfileItem(QObject *parent) :
    KodiModelItem(parent), m_lockMode(KodiModel::LockModeNone)
{
}

KodiModel::LockMode ProfileItem::lockMode() const
{
    return m_lockMode;
}

void ProfileItem::setLockMode(KodiModel::LockMode lockMode)
{
    m_lockMode = lockMode;
    emit lockModeChanged();
}

QVariant ProfileItem::data(int role) const
{
    if (role == KodiModel::RoleLockMode) {
        return m_lockMode;
    }

    return KodiModelItem::data(role);
}
