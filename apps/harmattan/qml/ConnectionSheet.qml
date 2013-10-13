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
import com.nokia.meego 1.0
import com.nokia.extras 1.1
import QtMobility.connectivity 1.2
import Xbmc 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: qsTr("Connect")
    rejectButtonText: qsTr("Cancel")
    acceptButton.enabled: itemView.currentIndex == 0 && hostList.currentIndex >= 0 || hostnameTextField.text != ""

    XbmcDiscovery {
        id: discovery
        continuousDiscovery: true
    }

    Connections {
        target: xbmc
        onConnectedChanged: {
            if(connected)
                connectionSheet.reject();
        }
    }

    content: Column {
        anchors.fill: parent
        spacing: 10

        VisualItemModel {
            id: itemModel

            Column {
                width: itemView.width
                height: itemView.height

                ListView {
                    id: hostList
                    width: itemView.width
                    height: itemView.height - manualAdd.height
                    model: xbmc.hostModel()
                    highlightFollowsCurrentItem: true

                    Column {
                        anchors.centerIn: parent
                        width: parent.width - 10
                        height: searchLabel + busyIndicator + spacing
                        spacing: 20
                        visible: hostList.count == 0

                        BusyIndicator {
                            id: busyIndicator
                            anchors.horizontalCenter: parent.horizontalCenter
                            platformStyle: BusyIndicatorStyle { size: "large" }
                            running: visible
                        }

                        Label {
                            id: searchLabel
                            width: parent.width
                            text: qsTr("Searching for XBMC hosts.") + "\n"
                                  + qsTr("Please enable the following options in the Services settings of XBMC:") + "\n- "
                                  + qsTr("Allow control of XBMC via HTTP") + "\n- "
                                  + qsTr("Allow programs on other systems to control XBMC") + "\n- "
                                  + qsTr("Announce these services to other systems via Zeroconf") + "\n"
                                  + qsTr("If you don't use Zeroconf, add a host manually.");
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    highlight: Rectangle {
                        width: parent.width
                        height: 88
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: theme.inverted ? "#4269a5" : "#3996e7" }
                            GradientStop { position: 1.0; color: theme.inverted ? "#29599c" : "#1082de" }
                        }

                        //color: theme.inverted ? "#1d87e0"
                    }

                    delegate: Item {
                        width: parent.width
                        height: 88
                        Column {
                            anchors.fill: parent

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    hostList.currentIndex = index;
                                }
                            }
                            Row {
                                id: itemRow
                                anchors {left: parent.left; top: parent.top; right: parent.right }
                                height: 88
                                anchors.leftMargin: 5
                                anchors.rightMargin: 5

                                Row {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width

                                    Label {
                                        id: mainText
                                        text: qsTr("XBMC on %1").arg(hostname)
                                        font.weight: Font.Bold
                                        font.pixelSize: 26
                                        width: itemRow.width - wolButton.width
                                        elide: Text.ElideRight
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    MediaControlButton {
                                        id: wolButton
                                        iconSource: "icon-m-common-alarm-screen"
                                        anchors.verticalCenter: parent.verticalCenter
                                        visible: hwaddr.length > 0
                                        onClicked: {
                                            xbmc.hostModel().wakeup(index)
                                            infoBanner.text = qsTr("Waking up %1...").arg(hostname)
                                            infoBanner.show()
                                        }
                                    }
                                }
                            }

                        }
                    }
                }
                Row {
                    width: parent.width
                    height: manualAdd.height
                    spacing: 10
                    Button {
                        id: removeButton
                        width: (parent.width - 10) / 2
                        text: qsTr("Remove Host");
                        onClicked: xbmc.hostModel().removeHost(hostList.currentIndex);
                        enabled: hostList.currentIndex !== -1
                    }
                    Button {
                        id: manualAdd
                        width: (parent.width - 10) / 2
                        text: qsTr("Add Host")
                        onClicked: itemView.incrementCurrentIndex()
                    }
                }
            }
            Column {
                height: itemView.height
                width: itemView.width
                Column {
                    height: parent.height - cancelManualAdd.height
                    width: parent.width

                    Label {
                        text: qsTr("Host:")
                    }
                    TextField {
                        width: parent.width
                        id: hostnameTextField
                        inputMethodHints: Qt.ImhSensitiveData|Qt.ImhNoAutoUppercase|Qt.ImhPreferLowercase|Qt.ImhUrlCharactersOnly
                    }
                    Item {
                        width: parent.width
                        height: 10
                    }

                    Label {
                        text: qsTr("HTTP Port:")
                    }
                    TextField {
                        text: "8080"
                        width: parent.width
                        id: portTextField
                        inputMask: "00000"
                        inputMethodHints: Qt.ImhDigitsOnly|Qt.ImhSensitiveData
                    }
                    Item {
                        width: parent.width
                        height: 10
                    }
                    Label {
                        text: qsTr("MAC Address for WakeOnLan (optional):")
                    }
                    TextField {
                        width: parent.width
                        id: macTextField
                        inputMask: "HH:HH:HH:HH:HH:HH;_"
                        inputMethodHints: Qt.ImhPreferNumbers
                    }
                }
                Button {
                    id: cancelManualAdd
                    text: qsTr("Back")
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: itemView.decrementCurrentIndex()
                }
            }
        }
        ListView {
            id: itemView
            model: itemModel
            anchors.fill: parent

            orientation: ListView.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
            preferredHighlightBegin: 0; preferredHighlightEnd: 0
            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapOneItem
            highlightMoveDuration: 200
            anchors.margins: appWindow.pageMargin
            interactive: false
        }



    }

    InfoBanner {
        id: infoBanner
        anchors.topMargin: 40
        z: 10
    }

    onAccepted: {
        console.log("sheet accepted")
        if(itemView.currentIndex == 0) {
            xbmc.hostModel().wakeup(hostList.currentIndex);
            xbmc.hostModel().connectToHost(hostList.currentIndex);
        } else {
            var newIndex = xbmc.hostModel().createHost(hostnameTextField.text, hostnameTextField.text, portTextField.text, macTextField.text);
            xbmc.hostModel().connectToHost(newIndex);
        }
        connectionSheet.destroy();
    }

    onRejected: {
        print("rejected");
        connectionSheet.destroy();
    }
}
