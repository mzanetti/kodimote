/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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

#include <QApplicationStateChangeEvent>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QGuiApplication>

#ifndef HARBOUR_BUILD
#include <QContact>
#include <QContactDisplayLabel>
#include <QContactManager>
#include <QContactPhoneNumber>
#endif

#include "sailfishhelper.h"
#include "libkodimote/kodi.h"
#include "libkodimote/kodihostmodel.h"
#include "libkodimote/settings.h"

#ifndef HARBOUR_BUILD
using namespace QtContacts;
#endif

SailfishHelper::SailfishHelper(QQuickView *quickView, Settings *settings, QObject *parent) :
    PlatformHelper(settings, parent),
    m_quickView(quickView),
    m_resourceSet(new ResourcePolicy::ResourceSet("player", 0, false, true))
{
    m_resourceSet->addResourceObject(new ResourcePolicy::ScaleButtonResource);
    QGuiApplication::instance()->installEventFilter(this);

    m_resourceSet->acquire();

    QDBusConnection systemBus = QDBusConnection::systemBus();
    systemBus.connect("org.ofono", "/ril_0", "org.ofono.VoiceCallManager", "CallAdded", this, SLOT(callAdded(QDBusMessage)));
    systemBus.connect("org.ofono", "/ril_0", "org.ofono.VoiceCallManager", "CallRemoved", this, SLOT(callEnded()));
}

bool SailfishHelper::canRaise() const
{
    return true;
}

void SailfishHelper::raise()
{
    m_quickView->raise();
}

bool SailfishHelper::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ApplicationStateChange) {
        Qt::ApplicationState state = static_cast<QApplicationStateChangeEvent*>(event)->applicationState();
        if (state == Qt::ApplicationActive) {
            m_resourceSet->acquire();
        } else {
            m_resourceSet->release();
        }
    }
    return QObject::eventFilter(obj, event);
}

void SailfishHelper::callAdded(const QDBusMessage &msg)
{
    QDBusArgument *arg = (QDBusArgument*)msg.arguments().at(1).data();
    if (arg->currentType() != QDBusArgument::MapType) {
        return;
    }

    QMap<QString, QString> properties = unpackMessage(*arg);

    QString phoneNumber = properties.value("LineIdentification");
    QString contactName = lookupContact(phoneNumber);

    QString caller = contactName.length() ? contactName : phoneNumber;
    callStarted(properties.value("State") == "incoming", caller);
}

QString SailfishHelper::lookupContact(QString phoneNumber)
{
#ifndef HARBOUR_BUILD
    QString matchNumber = phoneNumber.right(6);
    QContactManager contactManager;
    QList<QContact> contacts = contactManager.contacts();

    for (int i = 0; i < contacts.size(); ++i) {
        foreach (QContactPhoneNumber number, contacts.at(i).details<QContactPhoneNumber>()) {
             if (!number.isEmpty()) {
                QString phone = number.number();
                if (!phone.isEmpty() && phone.endsWith(matchNumber)) {
                    QList<QContactDisplayLabel> labels = contacts.at(i).details<QContactDisplayLabel>();
                    if (labels.size() > 0 && !labels.first().isEmpty()){
                        return labels.first().label();
                    }
                }
            }
        }
    }
#endif

    return "";
}

QMap<QString, QString> SailfishHelper::unpackMessage(const QDBusArgument &args)
{
    QMap<QString, QString> properties;

    args.beginMap();
    while (!args.atEnd()) {
        QString key;
        QVariant value;

        args.beginMapEntry();
        args >> key >> value;
        if (value.canConvert(QVariant::String)) {
            properties.insert(key, value.toString());
        }
        args.endMapEntry();
    }
    args.endMap();

    return properties;
}
