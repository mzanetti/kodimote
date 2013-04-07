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

import QtQuick 1.1
import org.kde.plasma.components 0.1
import org.kde.plasma.core 0.1 as PlasmaCore

import Xbmc 1.0

Item {
    id: root
    property int spacing
    property bool discovering: false
    state: "connect"

    signal hostSelected()

    XbmcDiscovery {
        id: discovery
        continuousDiscovery: root.discovering
    }

    ListView {
        id: hostListView
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            bottom: buttonRow.top
        }
        model: xbmc.hostModel()
        highlightFollowsCurrentItem: true
        highlight: Rectangle {
            width: hostListView.currentItem.width
            height: hostListView.currentItem.height
            color: "gray"

        }

        delegate: Item {
            height: hostLabel.height + root.spacing / 2
            width: parent.width
            Row {
                anchors.fill: parent
                spacing: root.spacing
                anchors.margins: root.spacing / 2
                Label {
                    id: hostLabel
                    text: hostname
                    width: parent.width - wakeupIcon.width - parent.spacing
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            print("setting cur ind", model.index)
                            hostListView.currentIndex = model.index
                        }
                    }
                }
                PlasmaCore.IconItem {
                    id: wakeupIcon
                    width: theme.iconSizes.small
                    height: theme.iconSizes.small
                    anchors.verticalCenter: parent.verticalCenter
                    source: "system-shutdown"
                    MouseArea { anchors.fill: parent;
                        onClicked: {
                            xbmc.hostModel().wakeup(index)
                        }
                    }
                }
            }
        }
    }
    Row {
        id: buttonRow
        spacing: root.spacing

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Row {
            width: parent.width - connectButton.width
            spacing: root.spacing

            Button {
                iconSource: "list-add"
                text: qsTr("Add")
                onClicked: {
                    root.state = "addhost"
                }
            }
            Button {
                iconSource: "list-remove"
                text: qsTr("Remove")
                enabled: hostListView.currentIndex >= 0
                onClicked: {
                    xbmc.hostModel().removeHost(hostListView.currentIndex);
                }
            }
        }
        Button {
            id: connectButton
            iconSource: "network-connect"
            text: qsTr("Connect")
            enabled: hostListView.currentIndex >= 0
            onClicked: {
                xbmc.hostModel().connectToHost(hostListView.currentIndex)
                root.hostSelected()
            }
        }
    }

    Grid {
        id: addHost
        columns: 2
        anchors.fill: parent
        spacing: root.spacing

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 200 }
        }

        Label {
            text: qsTr("Host:")
        }

        TextField {
            id: hostnameTextField
        }
        Label {
            text: qsTr("Port:")
        }
        TextField {
            id: portTextField
            text: "8080"
            inputMask: "00000"
        }
        Label {
            text: qsTr("MAC Address for WakeOnLan (optional):")
        }
        TextField {
            id: macTextField
            inputMask: "HH:HH:HH:HH:HH:HH;_"
        }
        Button {
            text: qsTr("OK")
            onClicked: {
                var newIndex = xbmc.hostModel().createHost(hostnameTextField.text, hostnameTextField.text, portTextField.text, macTextField.text);
                xbmc.hostModel().connectToHost(newIndex);
                root.state = "connect";
                root.hostSelected()
            }
        }
        Button {
            text: qsTr("Cancel")
            onClicked: {
                root.state = "connect";
            }
        }
    }

    states: [
        State {
            name: "addhost"
            PropertyChanges { target: addHost; opacity: 1 }
            PropertyChanges { target: hostListView; opacity: 0 }
            PropertyChanges { target: buttonRow; opacity: 0 }
        },
        State {
            name: "connect"
            PropertyChanges { target: addHost; opacity: 0 }
            PropertyChanges { target: hostListView; opacity: 1 }
            PropertyChanges { target: buttonRow; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            NumberAnimation { properties: "opacity"; duration: 200 }
        }
    ]
}
