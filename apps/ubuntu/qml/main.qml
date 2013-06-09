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

    property int pageMargins: units.gu(2)

    Rectangle {
        anchors.fill: parent
        color: "#ebebeb"
    }

    Loader {
        anchors.fill: parent
        sourceComponent: xbmc.connected ? mainComponent : noConnectionComponent
    }

    Component {
        id: noConnectionComponent
        Page {
            id: noConnectionPage
            title: "Select Host"
            anchors.fill: parent
            ListView {
                id: hostListView
                anchors.fill: parent
                model: xbmc.hostModel()
                delegate: ListItems.Standard {
                    id: hostDelegate
                    text: hostname
                    selected: index === ListView.view.currentIndex

                    onClicked: xbmc.hostModel().connectToHost(index)
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
                    continuousDiscovery: true
                }
            }


            tools: ToolbarActions {
                Action {
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
            Column {
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
            }
        }
    }

    Component {
        id: mainComponent
        Tabs {
            Tab {
                title: "Media"
                page: PageStack {
                    id: pageStack
                    Component.onCompleted: push(mainPage)
                    MainPage {
                        id: mainPage
                        visible: false

                    }
                }

            }
            Tab {
                title: "Now playing"
                page: NowPlayingPage {

                }
            }
            Tab {
                title: "Keypad"
                page: Keypad {

                }
            }
        }
    }


/*    Loader {
        id: mainLoader
        anchors.fill: parent
        sourceComponent: xbmc.connected ? tabsComponent : noConnectionComponent
    }

    FocusScope {
        focus: true
        Keys.onVolumeUpPressed: xbmc.volume += 5
        Keys.onVolumeDownPressed: xbmc.volume -= 5
    }

    Component {
        id: noConnectionComponent

        Item {
            Column {
                anchors.fill: parent
                anchors.margins: units.gu(1)
                spacing: units.gu(1)
                ListView {
                    id: hostListView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.height - buttonRow.height - parent.spacing
                    model: xbmc.hostModel()
                    delegate: ListItems.Standard {
                        text: hostname
                        selected: index === ListView.view.currentIndex

                        onClicked: ListView.view.currentIndex = index;
                    }
                    XbmcDiscovery {
                    }
                }
                Row {
                    id: buttonRow
                    spacing: units.gu(1)
                    width: parent.width

                    Button {
                        text: "Remove"
                        width: (parent.width - parent.spacing) / 2
                        onClicked: {
                            hostSelected(hostListView.currentIndex);
                        }
                    }
                    Button {
                        text: "Add"
                        width: (parent.width - parent.spacing) / 2
                        onClicked: {
                            hostSelected(hostListView.currentIndex);
                        }
                    }
                }
            }
         }

}
    Component {
        id: connectionDialog
        ConnectionSheet {

        }
    }

    Component {
        id: tabsComponent

        Tabs {
            id: tabs
            anchors.fill: parent
            parent: leftPane
            ItemStyle.class: "new-tabs"

            property ActionList tools: selectedTab.page.tools

            Tab {
                id: mainTab
                title: "Media"
                page: PageStack {
                    id: pageStack

                    Component.onCompleted: pageStack.push(mainPage)

                    function home() {
                        pageStack.clear();
                        pageStack.push(mainPage)
                    }

                    ToolbarActions {
                        id: browsingTools
                        Action {
                            text: "home"
                            onTriggered: {
                                pageStack.home();
                            }
                        }
                    }



                    MainPage {
                        id: mainPage
                    }
                }
            }

            Tab {
                id: nowPlayingTab
                title: "Now Playing"
                page: NowPlayingPage {
                    id: nowPlayingPage
                    timerActive: tabs.currentIndex === 1
                }
            }

            Tab {
                id: keypadTab

                title: "Keypad"

                page: Keypad {
                }
            }
        }
    }
*/}
