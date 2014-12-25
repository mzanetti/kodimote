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

#include "profiles.h"

#include <QTimer>

#include "kodiconnection.h"
#include "libraryitem.h"

Profiles::Profiles(QObject *parent) :
    KodiModel(parent)
{
}

QString Profiles::title() const
{
    return tr("Profiles");
}

int Profiles::count() const
{
    return m_list.size();
}

void Profiles::switchProfile(int index)
{
    if (index < 0 || index > m_list.size()) {
        return;
    }

    KodiModelItem *item = m_list.at(index);
    QVariantMap params;
    params.insert("profile", item->title());
    params.insert("prompt", true);

    KodiConnection::sendCommand("Profiles.LoadProfile", params);
}

void Profiles::refresh()
{
    KodiConnection::sendCommand("Profiles.GetProfiles", QVariantMap(), this, "listReceived");

    KodiConnection::sendCommand("Profiles.GetCurrentProfile", QVariantMap(), this, "currentProfileReceived");
}

void Profiles::currentProfileReceived(const QVariantMap &rsp)
{
    QString currentProfile = rsp.value("result").toMap().value("label").toString();
    if (m_currentProfile != currentProfile) {
        m_currentProfile = currentProfile;
        emit currentProfileChanged();
    }
}

QString Profiles::currentProfile() const
{
    return m_currentProfile;
}

int Profiles::currentProfileIndex() const
{
    int size = m_list.size();
    for (int i = 0; i < size; i++) {
        if (m_list.at(i)->title() == m_currentProfile) {
            return i;
        }
    }

    return -1;
}

void Profiles::listReceived(const QVariantMap &rsp)
{
    QList<KodiModelItem*> list;
    QVariantList responseList = rsp.value("result").toMap().value("profiles").toList();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        KodiModelItem *item = new KodiModelItem(this);
        item->setTitle(itemMap.value("label").toString());

        list.append(item);
    }

    beginResetModel();
    m_list = list;
    endResetModel();
    setBusy(false);
    emit countChanged();
}
