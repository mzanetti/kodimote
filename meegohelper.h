/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef MEEGOHELPER_H
#define MEEGOHELPER_H

#include <QObject>
#include <QTimer>
#include <qmsystem2/qmkeys.h>
#include <qmsystem2/qmdisplaystate.h>
#include <qmsystem2/qmbattery.h>
#include <resource/qt4/policy/resource-set.h>
#include <QtDBus/QDBusObjectPath>

class Settings;

class MeeGoHelper : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoHelper(Settings *settings, QObject *parent = 0);

signals:

private slots:
    void keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State);
    bool eventFilter(QObject *obj, QEvent *event);
    void callEvent(const QDBusObjectPath &, const QString &);
    void callTerminated();

    void connectionChanged(bool connected);

    void displaySettingChanged();

    void setBlankingPause();

private:
    Settings *m_settings;
    MeeGo::QmKeys m_keys;
    ResourcePolicy::ResourceSet *m_resouceSet;
    bool m_buttonsAcquired;
    QTimer m_displayBlankingTimer;
    MeeGo::QmDisplayState m_disaplyState;
    MeeGo::QmBattery *m_battery;
};

#endif // MEEGOHELPER_H
