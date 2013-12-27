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

Page {
    id: noConnectionPage
    property bool showList: !xbmc.connecting

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Add host")
                onClicked: pageStack.push(Qt.resolvedUrl("AddHostDialog.qml"))
            }
        }

        PageHeader {
            title: qsTr("Select Host")
        }

        SilicaListView {
            id: hostListView
            anchors.fill: parent
            model: xbmc.hostModel()
            anchors.margins: Theme.paddingMedium
            opacity: noConnectionPage.showList ? 1 : 0
            Behavior on opacity { NumberAnimation {} }

            delegate: ListItem {
                id: hostDelegate

                Label {
                    text: hostname
                }

                onClicked: {
                    noConnectionPage.showList = false
                    xbmc.hostModel().connectToHost(index)
                }
            }
            Column {
                spacing: Theme.paddingMedium
                visible: hostListView.count == 0
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
                continuousDiscovery: noConnectionPage.showList
            }
        }
    }
}
