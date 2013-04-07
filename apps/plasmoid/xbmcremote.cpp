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

#include "xbmcremote.h"
#include "declarativepopup.h"

#include "xbmc.h"
#include "player.h"

//#include <QAction>
#include <QPainter>

//#include <KCModuleInfo>
//#include <KConfigDialog>
//#include <KCModuleProxy>
//#include <KStandardDirs>

#include <Plasma/Containment>
#include <Plasma/DeclarativeWidget>

K_EXPORT_PLASMA_APPLET(xbmcremote, XbmcremoteApplet)

XbmcremoteApplet::XbmcremoteApplet(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
        m_panelContainment(true)
{
    // TODO: config
//    KNetworkManagerServicePrefs::instance(Knm::NETWORKMANAGEMENT_RCFILE);
//    setHasConfigurationInterface(true);

    // TODO: localisation
//    KGlobal::locale()->insertCatalog("libknetworkmanager");

    setPopupIcon("xbmcremote");

    //setPassivePopup(true); // FIXME: disable, only true for testing ...

    m_svg = new Plasma::Svg(this);
    m_svg->setImagePath("icons/xbmcremote");
    m_svg->setContainsMultipleImages(true);
}

XbmcremoteApplet::~XbmcremoteApplet()
{
}

// TODO: context options
//QList<QAction*> NetworkManagerApplet::contextualActions()
//{
//    return d->actions;
//}

void XbmcremoteApplet::init()
{
    Plasma::Containment * c = containment();

    /* When applet is not in panel the tooltip always appears when hovering
       any point of the popup's area, which is annoying. */
    if (c && (c->containmentType() == Plasma::Containment::PanelContainment ||
              c->containmentType() == Plasma::Containment::CustomPanelContainment)) {
        Plasma::ToolTipManager::self()->registerWidget(this);
        m_panelContainment = true;
    } else {
        m_panelContainment = false;
    }

    //kDebug();

    m_popup = new DeclarativePopup(this);
    // TODO: config
//    connect(d->m_popup, SIGNAL(configNeedsSaving()), this, SIGNAL(configNeedsSaving()));

    // TODO: context options
//    QAction* action = new QAction(i18nc("CheckBox to enable or disable networking completely", "Enable networking"), this);
//    action->setToolTip(i18nc("@info:tooltip tooltip for the 'Enable networking' checkbox", "Enable or disable the networking system"));
//    action->setCheckable(true);
//    action->setChecked(NetworkManager::isNetworkingEnabled());
//    connect(action, SIGNAL(triggered(bool)), d->m_popup, SLOT(networkingEnabledToggled(bool)));
//    connect(NetworkManager::notifier(), SIGNAL(networkingEnabledChanged(bool)),
//            action, SLOT(setChecked(bool)));

//    d->actions.append(action);

    setGraphicsWidget(m_popup);

    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectedChanged(bool)));
    connect(Xbmc::instance(), SIGNAL(activePlayerChanged()), SLOT(activePlayerChanged()));
}

//void NetworkManagerApplet::qmlCreationFinished()
//{
//}


//void NetworkManagerApplet::createConfigurationInterface(KConfigDialog *parent)
//{
//    // Add the networkmanager KCM pages to the applet's configdialog
//    m_kcmNM = new KCModuleProxy("kcm_networkmanagement");
//    m_kcmNMTray = new KCModuleProxy("kcm_networkmanagement_tray");
//    parent->addPage(m_kcmNM, m_kcmNM->moduleInfo().moduleName(),
//                    m_kcmNM->moduleInfo().icon());
//    parent->addPage(m_kcmNMTray, m_kcmNMTray->moduleInfo().moduleName(),
//                    m_kcmNMTray->moduleInfo().icon());

//    parent->resize(800, 600);

//    connect(parent, SIGNAL(applyClicked()), this, SLOT(saveConfiguration()));
//    connect(parent, SIGNAL(okClicked()), this, SLOT(saveConfiguration()));
//}

//void NetworkManagerApplet::saveConfiguration()
//{
//    // kcm_networkmanagement implicitly saves connection definition after
//    // editing is finished, so no need to call its save() method
//    // FIXME This just writes out changed values to ini file. kded module
//    // still continues to use old value
//    m_kcmNMTray->save();
//}

void XbmcremoteApplet::constraintsEvent(Plasma::Constraints constraints)
{
    // update the pixmap when a new size from kiconloader fits in, this makes sure the
    // icon is only displayed in sizes provides by KIconLoader, so we don't get blurry
    // icons
    if (constraints & (Plasma::SizeConstraint | Plasma::FormFactorConstraint)) {
        setAspectRatioMode(Plasma::Square);
        update();
    }
}

//void NetworkManagerApplet::updatePixmap()
//{
//    QString iconName = UiUtils::iconName(m_activeSystrayInterface);
//    if (iconName == m_currentPixmapIconName) {
//        return;
//    }

//    int s = UiUtils::iconSize(contentsRect().size());
//    m_currentPixmapIconName = iconName;
//    m_pixmap = KIcon(m_currentPixmapIconName).pixmap(s, s);
//    update();
//}

void XbmcremoteApplet::paintInterface(QPainter * p, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED( p );
    Q_UNUSED( option );

    if (!m_panelContainment) {
        /* To make applet's size matches the popup's size. The applet is the tray icon, which is 16x16 pixels size by default.*/
        adjustSize();
    }

    QString el = "logo";

    int s = qMin(contentsRect.width(), contentsRect.height());
    QRect rect(0, 0, s, s);
    QPixmap newIcon(QSize(s, s));
    newIcon.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&newIcon);

    m_svg->paint(&painter, rect, el);

    paintStatusOverlay(&painter, rect);

    painter.end();
    setPopupIcon(newIcon);
}

void XbmcremoteApplet::connectedChanged(bool connected)
{
    Q_UNUSED(connected)
    update();
}

void XbmcremoteApplet::activePlayerChanged()
{
    disconnect(this, SLOT(playStatusChanged()));
    if (Xbmc::instance()->activePlayer()) {
        connect(Xbmc::instance()->activePlayer(), SIGNAL(stateChanged()), SLOT(playStatusChanged()));
    }
}

void XbmcremoteApplet::playStatusChanged()
{
    update();
}

//bool NetworkManagerApplet::needToUpdateOverlay()
//{
//    SystrayOverlayOptions temp;

//    if (m_activeSystrayInterface && m_activeSystrayInterface->state() == NetworkManager::Device::NeedAuth) {
//        temp |= NetworkManagerApplet::NeedAuth;
//    }

//    if (m_totalActiveVpnConnections > 0) {
//        temp |= NetworkManagerApplet::Locked;
//    }

//    qreal opacity = m_overlayTimeline.currentValue();
//    if (!qFuzzyCompare(opacity, 1) && !m_previousStatusOverlay.isNull()) {
//        temp |= NetworkManagerApplet::PreviousOverlay;
//    }

//    if (!m_statusOverlay.isNull()) {
//        temp |= NetworkManagerApplet::StatusOverlay;
//    }

//    return (temp != m_systrayOverlayOption);
//}

//inline void NetworkManagerApplet::paintNeedAuthOverlay(QPainter *p, QRect &rect)
//{
//    // Needs authentication, show this in the panel
//    if (!m_activeSystrayInterface) {
//        m_systrayOverlayOption &= ~NetworkManagerApplet::NeedAuth;
//        kDebug() << "No active interface";
//        return;
//    }
//    /*
//    kDebug() << "Painting overlay ...>" << m_activeSystrayInterface->state();
//    */
//    if (m_activeSystrayInterface->state() == NetworkManager::Device::NeedAuth) {
//        //kDebug() << "Needing auth ...>";
//        int iconSize = (int)2*(rect.width()/3);

//        //kDebug() << "Security:iconSize" << iconSize;
//        QPixmap icon = KIcon("dialog-password").pixmap(iconSize);
//        QPoint pos = QPoint(rect.right() - iconSize,
//                            rect.bottom() - iconSize);

//        p->drawPixmap(pos, icon);
//        m_systrayOverlayOption |= NetworkManagerApplet::NeedAuth;
//    } else {
//        m_systrayOverlayOption &= ~NetworkManagerApplet::NeedAuth;
//    }
//}

inline void XbmcremoteApplet::paintStatusOverlay(QPainter *p, QRect &rect)
{
    if (Xbmc::instance()->activePlayer()) {
        QString el = Xbmc::instance()->activePlayer()->state();
        m_svg->paint(p, rect, el);
    }
}

///* Slots to react to changes from the daemon */
//void NetworkManagerApplet::deviceAdded(const QString & uni)
//{
//    Q_UNUSED(uni);
//    // update the tray icon
//    updateInterfaceList();

//    if (!m_activeInterface) {
//        if (m_interfaces.isEmpty()) {
//            setActiveInterface(0);
//        } else {
//            setActiveInterface(m_interfaces.first());
//        }
//        setActiveSystrayInterface(m_activeInterface);
//    }

//    setupInterfaceSignals();
//    interfaceConnectionStateChanged();
//}

//void NetworkManagerApplet::deviceRemoved(const QString & uni)
//{
//    // update the tray icon
//    updateInterfaceList();

//    if (uni == m_lastActiveInterfaceUni) {
//        if (m_interfaces.isEmpty()) {
//            setActiveInterface(0);
//        } else {
//            qSort(m_interfaces.begin(), m_interfaces.end(), networkInterfaceLessThan);
//            setActiveInterface(m_interfaces.first());
//            m_activeInterfaceState = NetworkManager::Device::UnknownState;
//        }
//    }
//    setupInterfaceSignals();
//    if (uni == m_lastActiveSystrayInterfaceUni) {
//        setActiveSystrayInterface(0);
//        resetActiveSystrayInterface();
//    } else {
//        interfaceConnectionStateChanged();
//    }
//    // kill any animations involving this interface
//}

//void NetworkManagerApplet::interfaceConnectionStateChanged()
//{
//    kDebug() << " +++ +++ +++ Connection State Changed +++ +++ +++";
//    NetworkManager::Device * interface = qobject_cast<NetworkManager::Device *>(sender());
//    if (interface) {
//        if (m_activeSystrayInterface && m_activeSystrayInterface->uni() != interface->uni()) {
//            switch (interface->state()) {
//            case NetworkManager::Device::Preparing:
//            case NetworkManager::Device::ConfiguringHardware:
//            case NetworkManager::Device::ConfiguringIp:
//            case NetworkManager::Device::CheckingIp:
//            case NetworkManager::Device::WaitingForSecondaries:
//                setActiveSystrayInterface(interface);
//                m_activeSystrayInterfaceState = NetworkManager::Device::UnknownState;
//                break;
//            default:
//                break;
//            }
//        }
//    } else if (!m_activeSystrayInterface) {
//        setActiveSystrayInterface(m_activeInterface);
//        m_activeSystrayInterfaceState = m_activeInterfaceState;
//    }
//    if (m_activeSystrayInterface) {
//        //kDebug() << "busy ... ?";
//        NetworkManager::Device::State state = m_activeSystrayInterface->state();
//        switch (state) {
//            case NetworkManager::Device::Preparing:
//            case NetworkManager::Device::ConfiguringHardware:
//            case NetworkManager::Device::ConfiguringIp:
//            case NetworkManager::Device::CheckingIp:
//            case NetworkManager::Device::WaitingForSecondaries:
//            case NetworkManager::Device::Deactivating:
//                if (m_activeSystrayInterfaceState != state) {
//                    setStatusOverlay(generateProgressStatusOverlay());
//                }
//                //setBusy(true);
//                break;
//            case NetworkManager::Device::NeedAuth:
//                //setBusy(false);
//                break;
//            case NetworkManager::Device::Activated:
//                //setBusy(false);
//                if (m_activeSystrayInterfaceState != state) {
//                    // We want to show the full circle a bit
//                    setStatusOverlay(generateProgressStatusOverlay());
//                    setStatusOverlay("task-complete");
//                    QTimer::singleShot(4000, this, SLOT(clearActivatedOverlay()));
//                }
//                if (m_activeSystrayInterface->uni() != m_activeInterface->uni()) {
//                    QTimer::singleShot(5000, this, SLOT(resetActiveSystrayInterface()));
//                }
//                break;
//            case NetworkManager::Device::UnknownState:
//                kDebug() << "UnknownState! should this happen?";
//            case NetworkManager::Device::Unmanaged:
//            case NetworkManager::Device::Unavailable:
//            case NetworkManager::Device::Failed:
//                if (m_activeSystrayInterfaceState != state) {
//                    setStatusOverlay("dialog-error");
//                }
//                QTimer::singleShot(4000, this, SLOT(resetActiveSystrayInterface()));
//                break;
//            case NetworkManager::Device::Disconnected:
//                if (m_activeSystrayInterfaceState != state) {
//                    setStatusOverlay("dialog-cancel");
//                }
//                QTimer::singleShot(2000, this, SLOT(resetActiveSystrayInterface()));
//                break;
//        }
//        m_activeSystrayInterfaceState = state;
//    }
//    updatePixmap();
//}

//void NetworkManagerApplet::toolTipAboutToShow()
//{
//    NetworkManager::DeviceList interfaces
//        = NetworkManager::networkInterfaces();
//    if (interfaces.isEmpty()) {
//        m_toolTip = Plasma::ToolTipContent(QString(),
//                                        i18nc("Tooltip sub text", "No network interfaces"),
//                                        KIcon("networkmanager").pixmap(IconSize(KIconLoader::Desktop))
//                                        );
//    } else {
//        qSort(interfaces.begin(), interfaces.end(), networkInterfaceLessThan);
//        bool hasActive = false;
//        bool iconChanged = false;
//        QString icon = "networkmanager";
//        QStringList lines;
//        foreach (NetworkManager::Device *iface, interfaces) {
//            if (iface->state() != NetworkManager::Device::Unavailable &&
//                iface->state() != NetworkManager::Device::Unmanaged) {
//                if (!lines.isEmpty()) {
//                    lines << QString();
//                }
//                hasActive = true;

//                QString deviceText = UiUtils::interfaceNameLabel(iface->uni());
//                lines << QString::fromLatin1("<b>%1</b>").arg(deviceText);

//                QString connectionName;
//                RemoteInterfaceConnection *conn = m_activatables->connectionForInterface(iface);
//                if (conn) {
//                    connectionName = conn->connectionName();
//                }

//                lines << QString("%1").arg(UiUtils::connectionStateToString(static_cast<NetworkManager::Device::State>(iface->state()), connectionName));
//                /*
//                IPv4Config ip4Config = iface->ipV4Config();
//                QList<IPv4Address> addresses = ip4Config.addresses();
//                if (!addresses.isEmpty()) {
//                    QHostAddress addr(addresses.first().address());
//                    QString currentIp = addr.toString();
//                    lines << i18nc("Display of the IP (network) address in the tooltip", "<font size=\"-1\">Address: %1</font>", currentIp);
//                }
//                */
//                // Show the first active connection's icon, otherwise the networkmanager icon
//                if (!iconChanged && iface->state() == NetworkManager::Device::Activated) {
//                    icon = UiUtils::iconName(iface);
//                    iconChanged = true; // we only want the first one
//                }
//            }
//        }

//        QString subText;
//        QString text;

//        /* VPN connections require at least one non-vpn active connection.
//           If there is no non-vpn active connection then there is no active VPN connection too.*/
//        if (hasActive) {
//            if (!m_activeVpnConnections.isEmpty()) {
//                lines << QString();
//                lines << QString::fromLatin1("<b>%1</b>").arg(i18n("VPN Connections"));
//                QMap<QUuid, QWeakPointer<RemoteInterfaceConnection> >::iterator i = m_activeVpnConnections.begin();
//                while (i != m_activeVpnConnections.end()) {
//                    RemoteInterfaceConnection *ic = i.value().data();
//                    if (!ic) {
//                        i = m_activeVpnConnections.erase(i);
//                    } else {
//                        lines << QString("%1").arg(UiUtils::connectionStateToString(ic->activationState(), ic->connectionName()));
//                        i++;
//                    }
//                }
//            }
//            subText = lines.join(QLatin1String("<br>"));
//        } else {
//            text = i18nc("tooltip, all interfaces are down", "Disconnected");

//            if (d->m_popup->property("hasWireless").toBool() && !NetworkManager::isWirelessEnabled()) {
//                subText = i18nc("tooltip, wireless is disabled in software", "Wireless disabled in software");
//            }
//            if (!NetworkManager::isNetworkingEnabled()) {
//                subText = i18nc("tooltip, all interfaces are down", "Networking disabled");
//            }
//            if (d->m_popup->property("hasWireless").toBool() && !NetworkManager::isWirelessHardwareEnabled()) {
//                subText = i18nc("tooltip, wireless is disabled by hardware", "Wireless disabled by hardware");
//            }

//        }
//        m_toolTip = Plasma::ToolTipContent(text,
//                                           subText,
//                                           KIcon(icon).pixmap(IconSize(KIconLoader::Desktop))
//                                           );
//    }
//    Plasma::ToolTipManager::self()->setContent(this, m_toolTip);
//}


//bool networkInterfaceLessThan(NetworkManager::Device *if1, NetworkManager::Device * if2)
//{
//    /*
//     * status merging algorithm
//     * In descending order of importance:
//     * - Connecting devices
//     *   - Cellular devices (because of cost)
//     *   - = PPP devices
//     *   - Ethernet devices
//     *   - Wireless devices
//     * - Connected devices
//     *   - order as above
//     * - Disconnected devices
//     *   - order as above
//     */
//    enum {  Connecting,
//            Connected,
//            Disconnected,
//            Unavailable }
//        if2status = Unavailable,
//        if1status = Unavailable;

//    switch (if1->state()) {
//        case NetworkManager::Device::Preparing:
//        case NetworkManager::Device::ConfiguringHardware:
//        case NetworkManager::Device::NeedAuth:
//        case NetworkManager::Device::ConfiguringIp:
//        case NetworkManager::Device::CheckingIp:
//        case NetworkManager::Device::WaitingForSecondaries:
//            if1status = Connecting;
//            break;
//        case NetworkManager::Device::Activated:
//            if1status = Connected;
//            break;
//        case NetworkManager::Device::Disconnected:
//            if1status = Disconnected;
//            break;
//        default: // all kind of unavailable
//            break;
//    }
//    switch (if2->state()) {
//        case NetworkManager::Device::Preparing:
//        case NetworkManager::Device::ConfiguringHardware:
//        case NetworkManager::Device::NeedAuth:
//        case NetworkManager::Device::ConfiguringIp:
//        case NetworkManager::Device::CheckingIp:
//        case NetworkManager::Device::WaitingForSecondaries:
//            if2status = Connecting;
//            break;
//        case NetworkManager::Device::Activated:
//            if2status = Connected;
//            break;
//        case NetworkManager::Device::Disconnected:
//            if2status = Disconnected;
//            break;
//        default: // all kind of disconnected
//            break;
//    }

//    bool lessThan = false;
//    switch (if1status) {
//        case Connecting:
//            lessThan = (if2status != Connecting || networkInterfaceSameConnectionStateLessThan(if1, if2));
//            //return true;//
//            break;
//        case Connected:
//            if ( if2status == Connecting) {
//               return false;
//            }
//            lessThan = ((if2status != Connected) || networkInterfaceSameConnectionStateLessThan(if1, if2));
//            break;
//        case Disconnected:
//            lessThan = false;
//            if ( if2status == Disconnected) {
//                lessThan = networkInterfaceSameConnectionStateLessThan(if1, if2);
//            }
//            if (if2status == Unavailable) {
//                lessThan = true;
//            }
//            break;
//        case Unavailable:
//            lessThan = false;
//            if ( if2status == Unavailable) {
//                lessThan = networkInterfaceSameConnectionStateLessThan(if1, if2);
//            }
//            break;
//    }

//    return lessThan;
//}

//bool networkInterfaceSameConnectionStateLessThan(NetworkManager::Device * if1, NetworkManager::Device * if2)
//{
//    bool lessThan = false;
//    switch (if1->type() ) {
//        case NetworkManager::Device::Ethernet:
//            switch (if2->type()) {
//                case NetworkManager::Device::Ethernet:
//                    lessThan = if1->uni() < if2->uni();
//                    break;
//                case NetworkManager::Device::Wifi:
//                    lessThan = true;
//                    break;
//                case NetworkManager::Device::Modem:
//                default:
//                    lessThan = false;
//                    break;
//            }
//            break;
//        case NetworkManager::Device::Wifi:
//            switch (if2->type()) {
//                case NetworkManager::Device::Ethernet:
//                    lessThan = false;
//                    break;
//                case NetworkManager::Device::Wifi:
//                    lessThan = if1->uni() < if2->uni();
//                    break;
//                case NetworkManager::Device::Modem:
//                    lessThan = false;
//                    break;
//                default:
//                    lessThan = true;
//                    break;
//            }
//            break;
//        case NetworkManager::Device::Modem:
//            switch (if2->type()) {
//                case NetworkManager::Device::Ethernet:
//                case NetworkManager::Device::Wifi:
//                    lessThan = true;
//                    break;
//                case NetworkManager::Device::Modem:
//                    lessThan = if1->uni() < if2->uni();
//                    break;
//                default:
//                    lessThan = true;
//                    break;
//            }
//            break;
//        default:
//            lessThan = false;
//        }
//    return lessThan;
//}

//void NetworkManagerApplet::managerWirelessEnabledChanged(bool)
//{
//    setupInterfaceSignals();
//}

//void NetworkManagerApplet::managerWirelessHardwareEnabledChanged(bool enabled)
//{
//    Q_UNUSED( enabled );
//    setupInterfaceSignals();
//    updatePixmap();
//}

//void NetworkManagerApplet::userNetworkingEnabledChanged(bool enabled)
//{
//    kDebug() << enabled;
//    NetworkManager::setNetworkingEnabled(enabled);
//    setupInterfaceSignals();
//}

//void NetworkManagerApplet::userWirelessEnabledChanged(bool enabled)
//{
//    kDebug() << enabled;
//    NetworkManager::setWirelessEnabled(enabled);
//    setupInterfaceSignals();
//}

//void NetworkManagerApplet::managerStatusChanged(NetworkManager::Status status)
//{
//    //kDebug() << "managerstatuschanged";
//    updateInterfaceList();
//    if (status == NetworkManager::Unknown) {
//        setActiveInterface(0);
//        setActiveSystrayInterface(0);
//    } else {
//        if (!m_interfaces.isEmpty()) {
//            qSort(m_interfaces.begin(), m_interfaces.end(), networkInterfaceLessThan);
//            setActiveInterface(m_interfaces.first());
//            setActiveSystrayInterface(m_activeInterface);
//        }
//    }
//    setupInterfaceSignals();
//    updatePixmap();
//}

//bool NetworkManagerApplet::hasInterfaceOfType(NetworkManager::Device::Type type)
//{
//    foreach (NetworkManager::Device * interface, m_interfaces) {
//        if (interface->type() == type) {
//            return true;
//        }
//    }
//    return false;
//}

//void NetworkManagerApplet::setStatusOverlay(const QPixmap& pix)
//{
//    m_previousStatusOverlay = m_statusOverlay;
//    m_statusOverlay = pix;
//    if (m_overlayTimeline.state() == QTimeLine::Running) {
//        m_overlayTimeline.stop();
//    }
//    m_overlayTimeline.start();
//}

//void NetworkManagerApplet::setStatusOverlay(const QString& name)
//{
//    int i_s = (int)contentsRect().width()/4;
//    int size = UiUtils::iconSize(QSizeF(i_s, i_s));
//    QPixmap pix = KIcon(name).pixmap(size);
//    setStatusOverlay(pix);
//}

//QPixmap NetworkManagerApplet::generateProgressStatusOverlay(const qreal state)
//{
//    int width = contentsRect().width();
//    int height = qMax(width / 4, 4);

//    QPixmap pix(width, height);
//    pix.fill(Qt::transparent);

//    QPainter p(&pix);
//    p.setRenderHint(QPainter::Antialiasing);
//    m_meterBgSvg->resizeFrame(pix.size());
//    m_meterBgSvg->paintFrame(&p, pix.rect());

//    QRectF innerRect = pix.rect();
//    innerRect.setWidth(innerRect.width() * state);
//    m_meterFgSvg->resizeFrame(innerRect.size());
//    m_meterFgSvg->paintFrame(&p, innerRect);

//    return pix;
//}

//QPixmap NetworkManagerApplet::generateProgressStatusOverlay()
//{
//    return generateProgressStatusOverlay(UiUtils::interfaceState(m_activeSystrayInterface));
//}

//QPixmap NetworkManagerApplet::generateVpnProgressStatusOverlay(const RemoteInterfaceConnection *ic)
//{
//    return generateProgressStatusOverlay(UiUtils::interfaceConnectionState(ic));
//}

//void NetworkManagerApplet::clearActivatedOverlay()
//{
//    if (m_activeInterface && static_cast<NetworkManager::Device::State>(m_activeInterface->state()) == NetworkManager::Device::Activated) {
//        // Clear the overlay, but only if we are still activated
//        setStatusOverlay(QPixmap());
//    }
//}

//void NetworkManagerApplet::activatableAdded(RemoteActivatable *activatable)
//{
//    RemoteInterfaceConnection *ic = qobject_cast<RemoteInterfaceConnection*>(activatable);
//    if (activatable->activatableType() == Knm::Activatable::VpnInterfaceConnection) {
//        connect(ic, SIGNAL(activationStateChanged(Knm::InterfaceConnection::ActivationState,Knm::InterfaceConnection::ActivationState)),
//                this, SLOT(vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState,Knm::InterfaceConnection::ActivationState)));
//	Knm::InterfaceConnection::ActivationState state = ic->activationState();
//	if (state != Knm::InterfaceConnection::Unknown) {
//            QMetaObject::invokeMethod(ic, "activationStateChanged", Q_ARG(Knm::InterfaceConnection::ActivationState, ic->oldActivationState()), Q_ARG(Knm::InterfaceConnection::ActivationState, state));
//	}
//    } else if (ic) {
//        connect(ic, SIGNAL(hasDefaultRouteChanged(bool)), SLOT(updateActiveInterface(bool)));
//        QMetaObject::invokeMethod(ic, "hasDefaultRouteChanged", Q_ARG(bool, ic->hasDefaultRoute()));
//    }
//}

//void NetworkManagerApplet::vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState oldState, Knm::InterfaceConnection::ActivationState newState)
//{
//    RemoteInterfaceConnection *ic = static_cast<RemoteInterfaceConnection*>(sender());
//    QUuid id = ic->connectionUuid();
//    switch (newState)
//    {
//        case Knm::InterfaceConnection::Activated:
//            m_totalActiveVpnConnections++;
//            setStatusOverlay(generateVpnProgressStatusOverlay(ic));
//            setStatusOverlay("task-complete");
//            QTimer::singleShot(4000, this, SLOT(clearActivatedOverlay()));
//            if (!m_activeVpnConnections.contains(id))
//                m_activeVpnConnections.insert(id, QWeakPointer<RemoteInterfaceConnection>(ic));
//            break;
//        case Knm::InterfaceConnection::Activating:
//            setStatusOverlay(generateVpnProgressStatusOverlay(ic));
//            m_activeVpnConnections.insert(id, QWeakPointer<RemoteInterfaceConnection>(ic));
//            break;
//        case Knm::InterfaceConnection::Unknown:
//            setStatusOverlay(QPixmap());
//            m_activeVpnConnections.remove(id);
//            if (oldState == Knm::InterfaceConnection::Activated && m_totalActiveVpnConnections > 0) {
//                m_totalActiveVpnConnections--;
//            }
//            break;
//    }
//    //kDebug() << newState << m_totalActiveVpnConnections;
//    update();
//}

//void NetworkManagerApplet::updateActiveInterface(bool hasDefaultRoute)
//{
//    RemoteInterfaceConnection *ic = qobject_cast<RemoteInterfaceConnection*>(sender());
//    if (hasDefaultRoute) {
//        // TODO: add support for VpnRemoteInterfaceConnection's, which have "any" as ic->deviceUni().
//        setActiveInterface(NetworkManager::findNetworkInterface(ic->deviceUni()));
//        connect(m_activeInterface, SIGNAL(destroyed(QObject*)), SLOT(_k_destroyed(QObject*)));
//        resetActiveSystrayInterface();
//    }
//}

//void NetworkManagerApplet::_k_destroyed(QObject *object)
//{
//    Q_UNUSED(object);
//    if (object == m_activeInterface) {
//        setActiveInterface(0);
//    }
//    if (object == m_activeSystrayInterface) {
//        setActiveSystrayInterface(0);
//    }
//}

//void NetworkManagerApplet::resetActiveSystrayInterface()
//{
//    if (m_activeInterface && m_activeSystrayInterface && m_activeSystrayInterface->uni() == m_activeInterface->uni()) {
//        return;
//    }
//    setActiveSystrayInterface(m_activeInterface);
//    m_activeSystrayInterfaceState = m_activeInterfaceState;
//    if (m_activeSystrayInterfaceState == NetworkManager::Device::Activated) {
//        setStatusOverlay(QPixmap());
//    }
//    interfaceConnectionStateChanged();
//}

//void NetworkManagerApplet::activatableRemoved(RemoteActivatable *activatable)
//{
//    if (activatable->activatableType() == Knm::Activatable::VpnInterfaceConnection) {
//        RemoteInterfaceConnection *ic = static_cast<RemoteInterfaceConnection*>(activatable);
//        m_activeVpnConnections.remove(ic->connectionUuid());
//        kDebug() << "activatable removed" << m_activeVpnConnections.count();
//    }
//    RemoteWirelessNetwork *rwic;
//    RemoteWirelessInterfaceConnection *rwic2;
//    rwic = qobject_cast<RemoteWirelessNetwork *>(activatable);
//    if(rwic) {
//        kDebug() << "removed network " + rwic->ssid();
//    } else {
//        rwic2 = qobject_cast<RemoteWirelessInterfaceConnection *>(activatable);
//        if(rwic2) {
//            kDebug() << "removed network " + rwic2->ssid();
//        } else {
//            kDebug() << "removed network any";
//        }
//    }
//}

//void NetworkManagerApplet::activatablesDisappeared()
//{
//    m_totalActiveVpnConnections = 0;
//    update();
//}

//void NetworkManagerApplet::setActiveInterface(NetworkManager::Device * device)
//{
//    m_activeInterface = device;

//    if (m_activeInterface) {
//        m_lastActiveInterfaceUni = m_activeInterface->uni();
//    }
//}

//void NetworkManagerApplet::setActiveSystrayInterface(NetworkManager::Device * device)
//{
//    m_activeSystrayInterface = device;

//    if (m_activeSystrayInterface) {
//        m_lastActiveSystrayInterfaceUni = m_activeSystrayInterface->uni();
//    }
//}

//void NetworkManagerApplet::updateInterfaceList()
//{
//    m_interfaces = NetworkManager::networkInterfaces();
//    setStatus(m_interfaces.isEmpty() ? Plasma::PassiveStatus : Plasma::ActiveStatus);
//}

#include "xbmcremote.moc"
