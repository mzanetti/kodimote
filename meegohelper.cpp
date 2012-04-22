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

#include "meegohelper.h"
#include "xbmc/xbmc.h"
#include "xbmc/videoplayer.h"
#include "xbmc/audioplayer.h"
#include "settings.h"
#include "xbmc/xbmcmodel.h"
#include "xbmc/xbmcdownload.h"

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

QTM_USE_NAMESPACE

MeeGoHelper::MeeGoHelper(Settings *settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_resouceSet(new ResourcePolicy::ResourceSet("player")),
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
    // We support this URL format: xbmc://host:port/hostname/macaddr where hostname and macaddr are optional
    int connectToIndex = -1;
    QStringList argList = QApplication::arguments();
    qDebug() << "got command line arguments:" << argList;
    if(argList.count() > 1 && !argList.at(1).startsWith('-')) {
        QUrl uri = QUrl(argList.at(1));
        if(uri.isValid()) {
            XbmcHost host;
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
            connectToIndex = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);

            // In case of a NFC chip we really want to connect, so wake up the target host.
            Xbmc::instance()->hostModel()->wakeup(connectToIndex);
        }
    }

    // Load stored hosts
    foreach(const XbmcHost &host, settings->hostList()) {
        int index = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        if(connectToIndex == -1 && host.address() == settings->lastHost().address()) {
            qDebug() << "reconnecting to" << host.hostname() << host.address() << host.username() << host.password();
            connectToIndex = index;
        }
    }
    if(connectToIndex != -1) {
        Xbmc::instance()->hostModel()->connectToHost(connectToIndex);
    }

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectionChanged(bool)));
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(hostRemoved()));
    connect(Xbmc::instance(), SIGNAL(downloadAdded(XbmcDownload*)), SLOT(downloadAdded(XbmcDownload*)));

    m_displayBlankingTimer.setInterval(60000);
    connect(&m_displayBlankingTimer, SIGNAL(timeout()), SLOT(setBlankingPause()));

    m_battery = new MeeGo::QmBattery(this);
    connect(m_battery, SIGNAL(chargerEvent(MeeGo::QmBattery::ChargerType)), SLOT(displaySettingChanged()));
    connect(settings, SIGNAL(keepDisplayLitChanged()), SLOT(displaySettingChanged()));

    m_transferUpdateTimer.setInterval(500);
    connect(&m_transferUpdateTimer, SIGNAL(timeout()), SLOT(updateTransfers()));

    displaySettingChanged();
}

bool MeeGoHelper::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate) {
        m_resouceSet->release();
        m_buttonsAcquired = false;
    } else if(event->type() == QEvent::WindowActivate){
        m_resouceSet->acquire();
        m_buttonsAcquired = true;
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
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() + 5);
        break;
    case MeeGo::QmKeys::VolumeDown:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() - 5);
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
            Xbmc::instance()->showNotification(tr("Incoming call"), caller);
        }
    }

    if(settings.changeVolumeOnCall()) {
        Xbmc::instance()->dimVolumeTo(settings.volumeOnCall());
    }

    if(settings.pauseVideoOnCall() && Xbmc::instance()->videoPlayer()->state() == "playing") {
        Xbmc::instance()->videoPlayer()->playPause();
        m_videoPaused = true;
    }

    if(settings.pauseMusicOnCall() && Xbmc::instance()->audioPlayer()->state() == "playing") {
        Xbmc::instance()->audioPlayer()->playPause();
        m_musicPaused = true;
    }

    QDBusConnection::systemBus().connect(QString(), param1.path(), "com.nokia.csd.Call.Instance", "Terminated", this, SLOT(callTerminated()));

}

void MeeGoHelper::callTerminated()
{
    if(m_settings->changeVolumeOnCall()) {
        Xbmc::instance()->restoreVolume();
    }

    if(m_videoPaused) {
        Xbmc::instance()->videoPlayer()->playPause();
    }
    if(m_musicPaused) {
        Xbmc::instance()->audioPlayer()->playPause();
    }
}

void MeeGoHelper::connectionChanged(bool connected)
{
    if(connected) {
        m_settings->addHost(*Xbmc::instance()->connectedHost());
        m_settings->setLastHost(*Xbmc::instance()->connectedHost());
    }

}

void MeeGoHelper::hostRemoved()
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

void MeeGoHelper::downloadAdded(XbmcDownload *download)
{
    qDebug() << "Download added";
    TransferUI::Transfer *transfer = m_transferClient->registerTransfer(download->source(), TransferUI::Client::TRANSFER_TYPES_DOWNLOAD, "Xbmcremote");
    transfer->setIcon(download->iconId());
    transfer->setName(download->label());

    m_transferMap.insert(download, transfer);
    transfer->setActive();

    connect(download, SIGNAL(progressChanged()), SLOT(downloadProgress()));
    connect(download, SIGNAL(finished(bool)), SLOT(downloadDone(bool)));
    connect(transfer, SIGNAL(cancel()), SLOT(cancelTransfer()));
    m_transferUpdateTimer.start();
}

void MeeGoHelper::downloadProgress()
{
    XbmcDownload *download = qobject_cast<XbmcDownload*>(sender());
    TransferUI::Transfer *transfer = m_transferMap.value(download);
}

void MeeGoHelper::downloadDone(bool success)
{
    XbmcDownload *download = qobject_cast<XbmcDownload*>(sender());
    if(success) {
        TransferUI::Transfer *transfer = m_transferMap.value(download);
        transfer->markCompleted(true);
    }

    m_transferMap.take(download)->deleteLater();
}

void MeeGoHelper::updateTransfers()
{
    foreach(XbmcDownload *download, m_transferMap.keys()) {
        TransferUI::Transfer *transfer = m_transferMap.value(download);
        transfer->setSize(download->total());
        transfer->setProgress(1.0 * download->progress()/ download->total());
    }
}

void MeeGoHelper::cancelTransfer()
{
    TransferUI::Transfer *transfer = static_cast<TransferUI::Transfer*>(sender());
    m_transferMap.key(transfer)->cancel();
}
