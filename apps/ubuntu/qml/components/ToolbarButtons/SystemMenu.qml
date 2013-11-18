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
import ".."

ToolbarButton {
    id: systemMenu
    text: qsTr("More")
    iconSource: "image://theme/navigation-menu"
    onTriggered: PopupUtils.open(menuComponent, systemMenu)

    Component {
        id: menuComponent
        Popover {
            id: menuPopover
            Column {
                width: parent.width
                height: childrenRect.height

                ListItems.Standard {
                    text: qsTr("About")
                    icon: XIcon {
                        name: "../images/info.svg"
                        height: parent.height
                        width: height
                        color: closeIcon.color
                    }

                    onClicked: {
                        PopupUtils.open(aboutDialogComponent, systemMenu)
                        PopupUtils.close(menuPopover)
                    }
                }
                ListItems.Standard {
                    text: qsTr("Disconnect")
                    icon: Icon {
                        id: closeIcon
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
                    icon: Icon {
                        name: "save"
                        height: parent.height
                        width: height
                    }
                    onClicked: {
                        PopupUtils.close(powerMenu)
                        xbmc.hibernate();
                    }
                }
            }
        }
    }

    Component {
        id: aboutDialogComponent

        Dialog {
            id: aboutDialog
            title: "Xbmcremote 1.4"
            text: "Michael Zanetti\nmichael_zanetti@gmx.net"

            Item {
                width: parent.width
                height: units.gu(40)
                Column {
                    id: contentColumn
                    anchors.fill: parent
                    spacing: units.gu(1)

                    UbuntuShape {
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: units.gu(6)
                        width: units.gu(6)
                        radius: "medium"
                        image: Image {
                            source: "../../images/xbmcremote.svg"
                        }
                    }

                    Flickable {
                        width: parent.width
                        height: parent.height - y - (closeButton.height + parent.spacing) * 3
                        contentHeight: gplLabel.implicitHeight
                        clip: true
                        Label {
                            id: gplLabel
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: "This program is free software: you can redistribute it and/or modify " +
                                  "it under the terms of the GNU General Public License as published by " +
                                  "the Free Software Foundation, either version 3 of the License, or " +
                                  "(at your option) any later version.\n\n" +

                                  "This program is distributed in the hope that it will be useful, " +
                                  "but WITHOUT ANY WARRANTY; without even the implied warranty of " +
                                  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the " +
                                  "GNU General Public License for more details.\n\n" +

                                  "You should have received a copy of the GNU General Public License " +
                                  "along with this program.  If not, see http://www.gnu.org/licenses/."
                        }
                    }
                    Button {
                        id: closeButton
                        width: parent.width
                        text: qsTr("Close")
                        onClicked: PopupUtils.close(aboutDialog)
                    }
                    Button {
                        id: donateButton
                        width: parent.width
                        text: "Donate";
                        onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2")
                    }
                    Button {
                        text: "flattr";
                        width: parent.width
                        onClicked: Qt.openUrlExternally("http://flattr.com/thing/412274/Xbmcremote")
                    }
                }
            }
        }
    }
}
