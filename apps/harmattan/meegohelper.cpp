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

#include "meegohelper.h"
#include "libkodimote/kodi.h"
#include "libkodimote/videoplayer.h"
#include "libkodimote/audioplayer.h"
#include "libkodimote/settings.h"
#include "libkodimote/kodimodel.h"
#include "libkodimote/kodidownload.h"

#include <QDebug>
#include <QApplication>
#include <policy/audio-resource.h>
#include <QtDBus/QDBusConnection>
#include <QDBusMessage>
#include <QUrl>
#include <QtContacts/QContactManager>
#include <QtContacts/QContactDetailFilter>
#include <QtContacts/QContactPhoneNumber>
#include <TransferUI/Transfer>
#include <QNetworkConfigurationManager>

QTM_USE_NAMESPACE

MeeGoHelper::MeeGoHelper(Settings *settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_resouceSet(new ResourcePolicy::ResourceSet("player")),
    m_mustConnect(false),
    m_videoPaused(false),
    m_musicPaused(false),
    m_transferClient(new TransferUI::Client(this))
{
    connect(&m_keys, SIGNAL(keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)), SLOT(keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State)));

    m_resouceSet->addResource(ResourcePolicy::ScaleButtonType);
    QApplication::instance()->installEventFilter(this);

    m_resouceSet->acquire();
    m_buttonsAcquired = true;


    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Coming", this, SLOT(callEvent(QDBusObjectPath,QString)));
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Created", this, SLOT(callEvent(QDBusObjectPath,QString)));

    // Are we launched with an URI as argument? e.g. by an NFC chip?
    // We support this URL format: kodi://host:port/hostname/macaddr where hostname and macaddr are optional
    int connectToIndex = -1;
    QStringList argList = QApplication::arguments();
    qDebug() << "got command line arguments:" << argList;
    if(argList.count() > 1 && !argList.at(1).startsWith('-')) {
        QUrl uri = QUrl(argList.at(1));
        if(uri.isValid()) {
            KodiHost host;
            host.setAddress(uri.host());
            host.setPort(uri.port());
            QString path = uri.path().right(uri.path().length() - 1);
            if(path.split('/').count() >= 1) {
                host.setHostname(path.split('/').first());
            }
            if(path.split('/').count() >= 2) {
                host.setHwAddr(path.split('/').at(1));
            }
            qDebug() << "Connecting to" << host.address() << ':' << host.port() << host.hostname() << host.hwAddr();
            connectToIndex = Kodi::instance()->hostModel()->insertOrUpdateHost(host);

            // In case of a NFC chip we really want to connect, so wake up the target host.
            Kodi::instance()->hostModel()->wakeup(connectToIndex);
        }
    }

    // Load stored hosts
    foreach(const KodiHost &host, settings->hostList()) {
        int index = Kodi::instance()->hostModel()->insertOrUpdateHost(host);
        if(connectToIndex == -1 && host.address() == settings->lastHost().address()) {
            qDebug() << "reconnecting to" << host.hostname() << host.address() << host.username() << host.password();
            connectToIndex = index;
        }
    }
    if(connectToIndex != -1) {
        m_connectToIndex = connectToIndex;
        if(QNetworkConfigurationManager().isOnline()) {
            internalConnect();
        }
        else {
            m_mustConnect = true;
        }
    }

    connect(Kodi::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectionChanged(bool)));
    connect(Kodi::instance()->hostModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(hostRemoved()));
    connect(Kodi::instance(), SIGNAL(downloadAdded(KodiDownload*)), SLOT(downloadAdded(KodiDownload*)));

    m_displayBlankingTimer.setInterval(60000);
    connect(&m_displayBlankingTimer, SIGNAL(timeout()), SLOT(setBlankingPause()));

    m_battery = new MeeGo::QmBattery(this);
    connect(m_battery, SIGNAL(chargerEvent(MeeGo::QmBattery::ChargerType)), SLOT(displaySettingChanged()));
    connect(settings, SIGNAL(keepDisplayLitChanged()), SLOT(displaySettingChanged()));

    displaySettingChanged();
}

void MeeGoHelper::internalConnect()
{
    Kodi::instance()->hostModel()->connectToHost(m_connectToIndex);
}

bool MeeGoHelper::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate) {
        m_resouceSet->release();
        m_buttonsAcquired = false;
    } else if(event->type() == QEvent::WindowActivate){
        m_resouceSet->acquire();
        m_buttonsAcquired = true;
        if(m_mustConnect) {
            internalConnect();
            m_mustConnect = false;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MeeGoHelper::keyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    // only handle presses, no releases for now...
    if(!m_buttonsAcquired || state != MeeGo::QmKeys::KeyDown) {
        return;
    }

    qDebug() << "keyEvent:" << key << state;
    switch(key) {
    case MeeGo::QmKeys::VolumeUp:
        Kodi::instance()->volumeUp();
        break;
    case MeeGo::QmKeys::VolumeDown:
        Kodi::instance()->volumeDown();
        break;
    default:
        break;
    }
}

void MeeGoHelper::callEvent(const QDBusObjectPath &param1, const QString &param2)
{
    qDebug() << "phone call event" << param1.path() << param2;

    Settings settings;

    if(settings.showCallNotifications()) {

        QDBusMessage msg = QDBusMessage::createMethodCall("com.nokia.csd", param1.path(), "com.nokia.csd.Call.Instance", "GetStatus");
        QDBusMessage reply = QDBusConnection::systemBus().call(msg);
        qDebug() << "call status:" << reply.arguments();

        if(reply.arguments().first().toInt() == 3) {

            QContactDetailFilter phoneFilter;
            phoneFilter.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
            phoneFilter.setValue(param2.right(6));
            phoneFilter.setMatchFlags(QContactFilter::MatchContains);
            qDebug() << "search contact";
            QContactManager contactManager;

            QString caller;
            if(contactManager.contacts(phoneFilter).count() > 0) {
                caller = contactManager.contacts(phoneFilter).first().displayLabel();
            } else {
                caller = param2;
            }

            qDebug() << "got contact" << caller;
            Kodi::instance()->sendNotification(tr("Incoming call"), caller);
        }
    }

    if(settings.changeVolumeOnCall()) {
        Kodi::instance()->dimVolumeTo(settings.volumeOnCall());
    }

    if(settings.pauseVideoOnCall() && Kodi::instance()->videoPlayer()->state() == "playing") {
        Kodi::instance()->videoPlayer()->playPause();
        m_videoPaused = true;
    }

    if(settings.pauseMusicOnCall() && Kodi::instance()->audioPlayer()->state() == "playing") {
        Kodi::instance()->audioPlayer()->playPause();
        m_musicPaused = true;
    }

    QDBusConnection::systemBus().connect(QString(), param1.path(), "com.nokia.csd.Call.Instance", "Terminated", this, SLOT(callTerminated()));

}

void MeeGoHelper::callTerminated()
{
    if(m_settings->changeVolumeOnCall()) {
        Kodi::instance()->restoreVolume();
    }

    if(m_videoPaused) {
        Kodi::instance()->videoPlayer()->playPause();
        m_videoPaused = false;
    }
    if(m_musicPaused) {
        Kodi::instance()->audioPlayer()->playPause();
        m_musicPaused = false;
    }
}

void MeeGoHelper::connectionChanged(bool connected)
{
    if(connected) {
        m_settings->addHost(*Kodi::instance()->connectedHost());
        m_settings->setLastHost(*Kodi::instance()->connectedHost());
    }

}

void MeeGoHelper::hostRemoved()
{
    // We need to check if all our stored hosts are still in hostList
    for(int i = 0; i < m_settings->hostList().count();) {
        bool found = false;
        for(int j = 0; j < Kodi::instance()->hostModel()->rowCount(QModelIndex()); ++j) {
            if(m_settings->hostList().at(i).address() == Kodi::instance()->hostModel()->get(j, "address").toString()) {
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

void MeeGoHelper::displaySettingChanged()
{
    qDebug() << m_battery->getChargerType();
    if(m_settings->keepDisplayLit() &&
            (m_battery->getChargerType() != MeeGo::QmBattery::None)) {
        qDebug() << "Disabling display blanking";
        m_displayBlankingTimer.start();
        setBlankingPause();
    } else {
        qDebug() << "Enabling display blanking";
        m_displayBlankingTimer.stop();
    }
}

void MeeGoHelper::setBlankingPause()
{
    m_disaplyState.setBlankingPause();
}

void MeeGoHelper::downloadAdded(KodiDownload *download)
{
    qDebug() << "Download added";
    TransferUI::Transfer *transfer = m_transferClient->registerTransfer(download->source(), TransferUI::Client::TRANSFER_TYPES_DOWNLOAD, "Kodimote");
    transfer->setIcon(download->iconId());
    transfer->setName(download->label());
    transfer->setSize(0);
    transfer->setProgress(0);

    m_transferMap.insert(download, transfer);

    connect(download, SIGNAL(progressChanged()), SLOT(downloadProgress()));
    connect(download, SIGNAL(started()), SLOT(downloadStarted()));
    connect(download, SIGNAL(finished(bool)), SLOT(downloadDone(bool)));
    connect(transfer, SIGNAL(cancel()), SLOT(cancelTransfer()));
    m_lastTransferUpdate = QDateTime::currentDateTime();
}

void MeeGoHelper::downloadProgress()
{
    // update the remote transfer window less often as our signal is called
    if(m_lastTransferUpdate.addMSecs(500) < QDateTime::currentDateTime()) {

        KodiDownload *download = qobject_cast<KodiDownload*>(sender());
        TransferUI::Transfer *transfer = m_transferMap.value(download);
        transfer->setSize(download->total());
        transfer->setProgress(1.0 * download->progress()/ download->total());

        m_lastTransferUpdate = QDateTime::currentDateTime();
    }
}

void MeeGoHelper::downloadStarted()
{
    KodiDownload *download = qobject_cast<KodiDownload*>(sender());
    TransferUI::Transfer *transfer = m_transferMap.value(download);
    transfer->setActive();
}

void MeeGoHelper::downloadDone(bool success)
{
    KodiDownload *download = qobject_cast<KodiDownload*>(sender());
    if(!download->isCancelled()) {
        TransferUI::Transfer *transfer = m_transferMap.value(download);
        transfer->markCompleted(success);
    }

    m_transferMap.take(download)->deleteLater();
}

void MeeGoHelper::cancelTransfer()
{
    TransferUI::Transfer *transfer = static_cast<TransferUI::Transfer*>(sender());
    m_transferMap.key(transfer)->cancel();
}
