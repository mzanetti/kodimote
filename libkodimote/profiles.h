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

#ifndef PROFILES_H
#define PROFILES_H

#include "kodimodel.h"

class Profiles : public KodiModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentProfile READ currentProfile NOTIFY currentProfileChanged)
    Q_PROPERTY(int currentProfileIndex READ currentProfileIndex NOTIFY currentProfileChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit Profiles(QObject *parent = 0);

    int count() const;
    QString currentProfile() const;
    int currentProfileIndex() const;
    Q_INVOKABLE QString title() const;

signals:
    void countChanged();
    void currentProfileChanged();

public slots:
    void refresh();
    void switchProfile(int index);

private:
    QString m_currentProfile;

private slots:
    void currentProfileReceived(const QVariantMap &rsp);
    void listReceived(const QVariantMap &rsp);
};

#endif // PROFILES_H
