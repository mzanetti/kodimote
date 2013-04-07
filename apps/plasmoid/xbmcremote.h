/*
Copyright 2008,2009 Will Stephenson <wstephenson@kde.org>
Copyright 2008-2010 Sebastian Kügler <sebas@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLASMA_NETWORKMANAGER_APPLET_H
#define PLASMA_NETWORKMANAGER_APPLET_H

class QAction;
class KCModuleProxy;

#include <QTimeLine>

#include <kdeversion.h>

#include <plasma/framesvg.h>
#include <plasma/svg.h>
#include <QtNetworkManager/manager.h>
#include <QtNetworkManager/accesspoint.h>

//#include <interfaceconnection.h>

#include <Plasma/PopupApplet>

#include <Plasma/ToolTipManager>

namespace Plasma
{
    class Applet;
} // namespace Plasma

class DeclarativePopup;

//class RemoteActivatable;
//class RemoteActivatableList;
//class RemoteInterfaceConnection;

class XbmcremoteApplet : public Plasma::PopupApplet
{
Q_OBJECT
public:
//    enum SystrayOverlayOption {
//        NoOverlay       = 0x00,
//        Locked          = 0x01,
//        PreviousOverlay = 0x02,
//        StatusOverlay   = 0x04,
//        NeedAuth        = 0x08,
//    };
//    Q_DECLARE_FLAGS(SystrayOverlayOptions, SystrayOverlayOption);

    XbmcremoteApplet(QObject * parent, const QVariantList & args);
    ~XbmcremoteApplet();
    /* reimp Plasma::Applet */
    void init();
    /* reimp Plasma::Applet */
    void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem * option,
                                            const QRect & rect);
    /* reimp Plasma::Applet */
    //QSizeF sizeHint(const Qt::SizeHint which, const QSizeF& constraint) const;
    /* reimp Plasma::Applet */
    //Qt::Orientations expandingDirections() const;
    /* reimp Plasma::Applet */
    void constraintsEvent(Plasma::Constraints constraints);

    //virtual void initExtenderItem(Plasma::ExtenderItem *);

    //void loadExtender();

//    QList<QAction*> contextualActions();

public Q_SLOTS:
protected:
//    void createConfigurationInterface(KConfigDialog *parent);

protected Q_SLOTS:
    // called by Plasma::ToolTipManager
//    void toolTipAboutToShow();

private Q_SLOTS:
    void connectedChanged(bool connected);
    void activePlayerChanged();
    void playStatusChanged();
//    void deviceAdded(const QString& = QString());
//    void deviceRemoved(const QString&);
//    void interfaceConnectionStateChanged();
//    void updatePixmap();
//    void clearActivatedOverlay();
//    void finishInitialization();
//    void activatableAdded(RemoteActivatable*);
//    void activatableRemoved(RemoteActivatable*);
//    void vpnActivationStateChanged(Knm::InterfaceConnection::ActivationState, Knm::InterfaceConnection::ActivationState);
//    void activatablesDisappeared();
//    // Request KCM module to persist changes
//    void saveConfiguration();
//    void updateActiveInterface(bool);
//    void resetActiveSystrayInterface();
//    void _k_destroyed(QObject *);
//    void setupAccessPointSignals(const QString &);
//    void qmlCreationFinished();
//    void updateWireless(bool checked);
private:
//    SystrayOverlayOptions m_systrayOverlayOption;

//    bool needToUpdateOverlay();
//    bool hasInterfaceOfType(NetworkManager::Device::Type type);
//    void setupInterfaceSignals();
//    QString svgElement(NetworkManager::Device *iface);
//    QString m_currentSvgElement;
//    QString m_currentPixmapIconName;

    void paintStatusOverlay(QPainter* p, QRect & rect);
//    void paintNeedAuthOverlay(QPainter* p, QRect & rect);
//    QPixmap generateProgressStatusOverlay(const qreal state);
//    QPixmap generateProgressStatusOverlay();
//    QPixmap generateVpnProgressStatusOverlay(const RemoteInterfaceConnection *ic);
//    void setStatusOverlay(const QPixmap&);
//    void setStatusOverlay(const QString&);
//    inline void setActiveInterface(NetworkManager::Device * device);
//    inline void setActiveSystrayInterface(NetworkManager::Device * device);

//    void updateInterfaceList();
//    NetworkManager::DeviceList m_interfaces;
//    Plasma::ToolTipContent m_toolTip;

//    RemoteActivatableList* m_activatables;

    bool m_panelContainment;

//    QPixmap m_pixmap;
//    QMap<QUuid, QWeakPointer<RemoteInterfaceConnection> > m_activeVpnConnections;
//    int m_totalActiveVpnConnections;

//    // For tracking which status we should show.
//    // This one is always the interface which has the default route
//    // or the first interface selected after sorting the current
//    // interface list using networkInterfaceLessThan().
//    // WARNING: do not directly assign this pointer, use setActiveInterface instead.
//    NetworkManager::Device* m_activeInterface;
//    // Interface used to update system tray icon. If we have only one interface
//    // then this one is always equals to m_activeInterfaceState.
//    // WARNING: do not directly assign this pointer, use setActiveSystrayInterface instead.
//    NetworkManager::Device* m_activeSystrayInterface;
//    NetworkManager::AccessPoint* m_accessPoint;

//    // Timeline controlling a connection progress overlay on the main icon
//    QTimeLine m_overlayTimeline;
//    QPixmap m_previousStatusOverlay;
//    QPixmap m_statusOverlay;

//    NetworkManager::Device::State m_activeInterfaceState;
//    NetworkManager::Device::State m_activeSystrayInterfaceState;

//    QString m_lastActiveInterfaceUni;
//    QString m_lastActiveSystrayInterfaceUni;

//    ///embedded KCM modules in the configuration dialog
//    KCModuleProxy* m_kcmNM;
//    KCModuleProxy* m_kcmNMTray;

    Plasma::Svg* m_svg;
//    Plasma::Svg* m_svgMobile;
//    Plasma::FrameSvg* m_meterBgSvg;
//    Plasma::FrameSvg* m_meterFgSvg;

//    class Private;
//    Private * const d;

    DeclarativePopup * m_popup;

};
//Q_DECLARE_OPERATORS_FOR_FLAGS(NetworkManagerApplet::SystrayOverlayOptions)

#endif
