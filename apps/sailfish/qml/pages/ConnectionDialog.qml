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
import Sailfish.Silica 1.0
import Xbmc 1.0

Dialog {
    id: connectionDialog
    canAccept: hostList.currentIndex >= 0

    onAccepted: {
        xbmc.hostModel().wakeup(hostList.currentIndex);
        xbmc.hostModel().connectToHost(hostList.currentIndex);
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Add host")
                onClicked: pageStack.push(Qt.resolvedUrl("AddHostDialog.qml"))
            }
        }

        DialogHeader {
            id: header
            acceptText: qsTr("Connect")
            cancelText: qsTr("Cancel")
        }

        SilicaListView {
            id: hostList
            anchors { top: header.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
            model: xbmc.hostModel()
            anchors.margins: Theme.paddingMedium
            opacity: count > 0 ? 1 : 0
            Behavior on opacity { NumberAnimation {} }
            highlightFollowsCurrentItem: true

            delegate: ListItem {
                id: hostDelegate
                contentHeight: Theme.itemSizeMedium
                highlighted: down || menuOpen || hostList.currentIndex == index

                menu: contextMenuComponent
                onClicked: {
                    hostList.currentIndex = index
                }

                Label {
                    text: hostname
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeLarge
                }
            }

            Component {
                id: contextMenuComponent
                ContextMenu {
                    MenuItem {
                        text: qsTr("Remove Host")
                        onClicked: xbmc.hostModel().removeHost(hostList.currentIndex);
                    }
                }
            }
        }

        Column {
            spacing: Theme.paddingMedium
            visible: hostList.count == 0
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            BusyIndicator {
                running: parent.visible
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                font.pixelSize: Theme.fontSizeMedium
                width: parent.width
                wrapMode: Text.WordWrap
                text: qsTr("Searching for XBMC hosts.") + "\n" + "\n"
                      + qsTr("Please enable the following options in the Services settings of XBMC:") + "\n- "
                      + qsTr("Allow control of XBMC via HTTP") + "\n- "
                      + qsTr("Allow programs on other systems to control XBMC") + "\n- "
                      + qsTr("Announce these services to other systems via Zeroconf") + "\n"
                      + qsTr("If you don't use Zeroconf, add a host manually.");
            }
        }

        XbmcDiscovery {
            continuousDiscovery: true
        }
    }
}
