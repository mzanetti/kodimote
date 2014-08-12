/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
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

#include "declarativepopup.h"
#include "settings.h"

#include <KDebug>
//#include <KToolInvocation>
#include <KStandardDirs>
#include <Plasma/Separator>

#include <QtDeclarative>

#include "xbmc.h"

DeclarativePopup::DeclarativePopup(QGraphicsWidget *parent) :
    Plasma::DeclarativeWidget(parent)
{
    qmlRegisterType<Plasma::Separator>("Separator", 0, 1, "Separator");

    m_settings = new Settings(this);

    setInitializationDelayed(true);
    m_rootContext = engine()->rootContext();
    m_rootContext->setContextProperty("xbmc", Xbmc::instance());
    m_rootContext->setContextProperty("popupState", "closed");
    m_rootContext->setContextProperty("settings", m_settings);


    connect(this, SIGNAL(finished()), this, SLOT(qmlCreationFinished()));
    connect(parent, SIGNAL(newStatus(Plasma::ItemStatus)), this, SLOT(newStatus(Plasma::ItemStatus)));
    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), this, SLOT(connectionChanged(bool)));

//    readConfig();

    setQmlPath(KStandardDirs::locate("data", "xbmcremote/qml/PopupDialog.qml"));
}

void DeclarativePopup::newStatus(Plasma::ItemStatus status)
{
    qDebug() << "status changed" << status;
    if (status == Plasma::NeedsAttentionStatus) {
        m_rootContext->setContextProperty("popupState", "open");
    } else {
        m_rootContext->setContextProperty("popupState", "closed");
    }
}

//void DeclarativeNMPopup::readConfig()
//{
//    kDebug();
//    KNetworkManagerServicePrefs::self()->readConfig();
//    KConfigGroup config(KNetworkManagerServicePrefs::self()->config(), QLatin1String("General"));

//    if (config.readEntry(QLatin1String("DetailKeys"), QStringList()).isEmpty()) {
//        QStringList keys;
//        keys << "interface:type"
//             << "interface:status"
//             << "ipv4:address"
//             << "interface:bitrate"
//             << "interface:name"
//             << "interface:hardwareaddress"
//             << "interface:driver"
//             << "wireless:ssid"
//             << "wireless:accesspoint"
//             << "wireless:band"
//             << "wireless:channel"
//             << "mobile:operator"
//             << "mobile:quality"
//             << "mobile:technology";

//        config.writeEntry(QLatin1String("DetailKeys"), keys);
//        config.sync();
//    }

//    //m_networkingCheckBox->setChecked(NetworkManager::isNetworkingEnabled());
//    m_rootContext->setContextProperty("wirelessChecked", NetworkManager::isWirelessEnabled());
//    m_rootContext->setContextProperty("wirelessEnabled", NetworkManager::isWirelessHardwareEnabled());

//    /*m_showMoreButton->setEnabled(NetworkManager::isNetworkingEnabled() &&
//                                 NetworkManager::isWirelessEnabled());*/

//    m_rootContext->setContextProperty("mobileChecked", NetworkManager::isWwanEnabled());
//    m_rootContext->setContextProperty("mobileEnabled", NetworkManager::isWwanHardwareEnabled());
//    /**
//    m_wwanCheckBox->nativeWidget()->setCheckState(NetworkManager::isWwanEnabled() ? Qt::Checked : Qt::Unchecked);
//    m_wwanCheckBox->setEnabled(NetworkManager::isWwanHardwareEnabled());

//    foreach(InterfaceItem * i, m_interfaces) {
//        i->setNameDisplayMode(InterfaceItem::InterfaceName);
//    }

//    QString version = NetworkManager::version();
//    if (version.isEmpty()) {
//        if (!m_warning) {
//            m_warning = new Plasma::Label(this);
//        }
//        m_warning->setText(i18nc("Warning about wrong NetworkManager version", "NetworkManager is not running. Please start it."));
//        m_tab1Layout->addItem(m_warning, 10, 0);
//    } else if (compareVersions(version, QString(MINIMUM_NM_VERSION_REQUIRED)) < 0) {
//        if (!m_warning) {
//            m_warning = new Plasma::Label(this);
//        }
//        m_warning->setText(i18nc("Warning about wrong NetworkManager version", "We need at least NetworkManager-%1 to work properly, found '%2'. Please upgrade to a newer version.", QString(MINIMUM_NM_VERSION_REQUIRED), version));
//        m_tab1Layout->addItem(m_warning, 10, 0);
//    } else if (m_warning) {
//        m_warning->deleteLater();
//        m_warning = 0;
//    }
//    **/
//}
