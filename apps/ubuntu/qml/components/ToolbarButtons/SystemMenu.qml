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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems

ToolbarButton {
    id: systemMenu
    text: qsTr("More")
    iconSource: "image://theme/properties"
    onTriggered: PopupUtils.open(menuComponent, systemMenu)

    Component {
        id: menuComponent
        Popover {
            id: menuPopover
            Column {
                width: parent.width
                height: childrenRect.height

                ListItems.Standard {
                    text: qsTr("Disconnect")
                    icon: Icon {
                        name: "close"
                        height: parent.height
                        width: height
                    }
                    onClicked: {
                        xbmc.disconnectFromHost();
                    }
                }
                ListItems.Standard {
                    text: qsTr("Power")
                    icon: Icon {
                        name: "system-shutdown"
                        height: parent.height
                        width: height
                    }
                    onClicked: {
                        PopupUtils.open(powerMenuComponent, systemMenu)
                        PopupUtils.close(menuPopover)
                    }
                }
            }
        }
    }

    Component {
        id: powerMenuComponent
        Popover {
            id: powerMenu

            Column {
                width: parent.width
                height: childrenRect.height

                ListItems.Standard {
                    text: qsTr("Quit")
                    icon: Icon {
                        name: "system-log-out"
                        height: parent.height
                        width: height
                    }
                    onClicked: {
                        xbmc.quit();
                        PopupUtils.close(powerMenu)
                    }
                }
                ListItems.Standard {
                    text: qsTr("Shutdown")
                    icon: Icon {
                        name: "system-shutdown"
                        height: parent.height
                        width: height
                    }
                    visible: xbmc.canShutdown
                    onClicked: {
                        xbmc.quit();
                        PopupUtils.close(powerMenu)
                    }
                }
                ListItems.Standard {
                    text: qsTr("Reboot")
                    icon: Icon {
                        name: "system-restart"
                        height: parent.height
                        width: height
                    }
                    visible: xbmc.canReboot
                    onClicked: {
                        xbmc.reboot();
                        PopupUtils.close(powerMenu)
                    }
                }
                ListItems.Standard {
                    text: qsTr("Suspend")
                    icon: Icon {
                        name: "torch-off"
                        height: parent.height
                        width: height
                    }
                    visible: xbmc.canSuspend
                    onClicked: {
                        PopupUtils.close(powerMenu)
                        xbmc.suspend();
                    }
                }
                ListItems.Standard {
                    text: qsTr("Hibernate")
                    visible: xbmc.canHibernate
                    onClicked: {
                        PopupUtils.close(powerMenu)
                        xbmc.hibernate();
                    }
                }
            }
        }
    }
}
