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
import Xbmc 1.0

MainView {
    id: appWindow

    headerColor: "#1b62c8"
    backgroundColor: "#0c2e71"
    footerColor: "#0a2663"


    property int pageMargins: units.gu(2)

    focus: true
    Keys.onVolumeUpPressed: {
        xbmc.volume += 5;
    }

    Keys.onVolumeDownPressed: {
        xbmc.volume -= 5;
    }

    Loader {
        anchors.fill: parent
        sourceComponent: xbmc.connected ? mainComponent : noConnectionComponent
    }

    Connections {
        target: xbmc
        onConnectingChanged: print("connecting.....", xbmc.connecting)
    }

    Component {
        id: noConnectionComponent
        Page {
            id: noConnectionPage
            title: "Select Host"
            anchors.fill: parent
            property bool showList: !xbmc.connecting

            ListView {
                id: hostListView
                anchors.fill: parent
                model: xbmc.hostModel()
                opacity: noConnectionPage.showList ? 1 : 0
                Behavior on opacity { UbuntuNumberAnimation {} }

                delegate: ListItems.Standard {
                    id: hostDelegate
                    text: hostname

                    onClicked: {
                        noConnectionPage.showList = false
                        xbmc.hostModel().connectToHost(index)
                    }

                    onPressAndHold: {
                        var obj = PopupUtils.open(removePopoverComponent, hostDelegate)
                        obj.removeClicked.connect(function() {
                            xbmc.hostModel().removeHost(index)
                            PopupUtils.close(obj)
                        })
                        obj.wakeupClicked.connect(function() {
                            xbmc.hostModel().wakeup(index)
                            PopupUtils.close(obj)
                        })
                    }
                }
                Column {
                    spacing: units.gu(5)
                    visible: hostListView.count == 0
                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: units.gu(1)
                        verticalCenter: parent.verticalCenter
                    }

                    ActivityIndicator {
                        running: parent.visible
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: units.gu(5)
                        height: width
                    }

                    Label {
                        fontSize: "medium"
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
            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: units.gu(2)
                }
                height: childrenRect.height
                spacing: units.gu(2)
                opacity: noConnectionPage.showList ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation {} }

                Label {
                    id: label
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: xbmc.connectionError
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
                Button{
                    text: qsTr("Cancel")
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    onClicked: {
                        noConnectionPage.showList = true
                    }
                }
            }


            tools: ToolbarItems {
                ToolbarButton {
                    text: "add"
                    iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/add.svg"
                    onTriggered: {
                        PopupUtils.open(addHostComponent, noConnectionPage)
                    }
                }
            }
        }
    }
    Component {
        id: removePopoverComponent
        Popover {
            id: popover
            signal removeClicked()
            signal wakeupClicked()
            Column {
                height: childrenRect.height
                width: parent.width
                ListItems.Standard {
                    text: qsTr("Remove")
                    icon: "/usr/share/icons/ubuntu-mobile/actions/scalable/delete.svg"
                    onClicked: popover.removeClicked()
                }
                ListItems.Standard {
                    text: qsTr("Wake up")
                    icon: "/usr/share/icons/ubuntu-mobile/actions/scalable/torch-on.svg"
                    onClicked: popover.wakeupClicked()
                }
            }
        }
    }

    Component {
        id: addHostComponent
        Dialog {
            id: addHostDialog
            title: qsTr("Add host")
            Item {
                width: parent.width
                height: units.gu(40)

                Flickable {
                    width: parent.width
                    height: parent.height
                    contentHeight: addColumn.height
                    clip: true
                    interactive: contentHeight > height

                    Column {
                        id: addColumn
                        width: parent.width
                        spacing: units.gu(1)
                        Label {
                            text: qsTr("Name:")
                        }
                        TextField {
                            id: nameTextField
                            width: parent.width
                        }
                        Label {
                            text: qsTr("Hostname or IP Address:")
                        }
                        TextField {
                            id: addressTextField
                            width: parent.width
                        }
                        Label {
                            text: qsTr("Port:")
                        }
                        TextField {
                            id: portTextField
                            text: "8080"
                            width: parent.width
                        }
                        Label {
                            text: qsTr("Mac Address:")
                        }
                        TextField {
                            id: macTextField
                            width: parent.width
                            inputMask: "HH:HH:HH:HH:HH:HH;_"
                        }
                        Row {
                            width: parent.width
                            spacing: units.gu(1)
                            Button {
                                text: qsTr("Cancel")
                                width: (parent.width - parent.spacing) / 2
                                onClicked: PopupUtils.close(addHostDialog)
                            }
                            Button {
                                text: qsTr("OK")
                                width: (parent.width - parent.spacing) / 2
                                color: "#dd4814"
                                onClicked: {
                                    xbmc.hostModel().createHost(nameTextField.text, addressTextField.text, portTextField.text, macTextField.text)
                                    PopupUtils.close(addHostDialog)
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: Qt.inputMethod.keyboardRectangle.height
                        }
                    }
                }
            }
        }
    }

    property bool showToolbars: true
    function resetToolBars() {
        showToolbars = false;
        showToolbars = true;
    }

    Component {
        id: mainComponent
        Tabs {
            id: tabs
            property int oldTabIndex: -1
            onSelectedTabIndexChanged: {
                if (oldTabIndex > 0 && selectedTabIndex == 0) {
                    resetToolBars()
                }
                oldTabIndex = selectedTabIndex;
            }
            Tab {
                title: "Media"
                page: PageStack {
                    id: pageStack
                    Component.onCompleted: push(mainPage)
                    onCurrentPageChanged: resetToolBars()
                    MainPage {
                        id: mainPage
                        visible: false

                    }
                }

            }
            Tab {
                title: "Now playing"
                page: NowPlayingPage {
                    timerActive: tabs.selectedTabIndex === 1
                }
            }
            Tab {
                title: "Keypad"
                page: Keypad {

                }
            }
        }
    }
}
