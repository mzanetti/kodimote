/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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
#include "libxbmcremote/xbmc.h"
#include "libxbmcremote/xbmchostmodel.h"
#include "libxbmcremote/videoplayer.h"
#include "libxbmcremote/audioplayer.h"
#include "libxbmcremote/settings.h"

using namespace QtContacts;

SailfishHelper::SailfishHelper(Settings *settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_resourceSet(new ResourcePolicy::ResourceSet("player", 0, false, true))
{
    m_resourceSet->addResourceObject(new ResourcePolicy::ScaleButtonResource);
    QGuiApplication::instance()->installEventFilter(this);

    m_resourceSet->acquire();

    QDBusConnection systemBus = QDBusConnection::systemBus();
    systemBus.connect("org.ofono", "/ril_0", "org.ofono.VoiceCallManager", "CallAdded", this, SLOT(callAdded(QDBusMessage)));
    systemBus.connect("org.ofono", "/ril_0", "org.ofono.VoiceCallManager", "CallRemoved", this, SLOT(callRemoved()));

    // Load stored hosts
    QString lastHostAddress = settings->lastHost().address();
    foreach(const XbmcHost &host, settings->hostList()) {
        int index = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        if(host.address() == lastHostAddress) {
            qDebug() << "reconnecting to" << host.hostname() << host.address() << host.username() << host.password();
            Xbmc::instance()->hostModel()->connectToHost(index);
        }
    }

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectionChanged(bool)));
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(hostRemoved()));
}

void SailfishHelper::connectionChanged(bool connected)
{
    if(connected) {
        m_settings->addHost(*Xbmc::instance()->connectedHost());
        m_settings->setLastHost(*Xbmc::instance()->connectedHost());
    }
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

void SailfishHelper::hostRemoved()
{
    // We need to check if all our stored hosts are still in hostList
    for(int i = 0; i < m_settings->hostList().count();) {
        bool found = false;
        for(int j = 0; j < Xbmc::instance()->hostModel()->rowCount(QModelIndex()); ++j) {
            if(m_settings->hostList().at(i).address() == Xbmc::instance()->hostModel()->get(j, "address").toString()) {
                found = true;
                break;
            }
        }
        if(!found) {
            m_settings->removeHost(m_settings->hostList().at(i));
            qDebug() << "removed host" << i;
        } else {
            ++i;
        }
    }
}

void SailfishHelper::callAdded(const QDBusMessage &msg)
{
    qDebug() << "call";
    QDBusArgument *arg = (QDBusArgument*)msg.arguments().at(1).data();
    if (arg->currentType() != QDBusArgument::MapType) {
        return;
    }

    Xbmc *xbmc = Xbmc::instance();
    QMap<QString, QString> properties = unpackMessage(*arg);

    qDebug() << properties;
    qDebug() << properties.value("State");
    qDebug() << m_settings->showCallNotifications();
    if (properties.value("State") == "incoming" && m_settings->showCallNotifications()) {
        QString phoneNumber = properties.value("LineIdentification");
        QString contactName = lookupContact(phoneNumber);

        QString caller = contactName.length() ? contactName : phoneNumber;
        xbmc->sendNotification(tr("Incoming call"), caller);
    }

    if(m_settings->changeVolumeOnCall()) {
        xbmc->dimVolumeTo(m_settings->volumeOnCall());
    }

    if(m_settings->pauseVideoOnCall() && xbmc->videoPlayer()->state() == "playing") {
        xbmc->videoPlayer()->playPause();
        m_videoPaused = true;
    }

    if(m_settings->pauseMusicOnCall() && xbmc->audioPlayer()->state() == "playing") {
        xbmc->audioPlayer()->playPause();
        m_musicPaused = true;
    }
}

void SailfishHelper::callRemoved()
{
    qDebug() << "call removed";
    if(m_settings->changeVolumeOnCall()) {
        Xbmc::instance()->restoreVolume();
    }

    if(m_videoPaused) {
        Xbmc::instance()->videoPlayer()->playPause();
        m_videoPaused = false;
    }
    if(m_musicPaused) {
        Xbmc::instance()->audioPlayer()->playPause();
        m_musicPaused = false;
    }
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
