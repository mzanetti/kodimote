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
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItems
import Xbmc 1.0
import "components"

MainView {
    id: appWindow

    headerColor: "#1b62c8"
    backgroundColor: "#0c2e71"
    footerColor: "#0a2663"

    property bool overrideColor: false

    property int pageMargins: units.gu(2)
    property var inputDialog

    useDeprecatedToolbar: false

    focus: true
    Keys.onVolumeUpPressed: {
        xbmc.volumeUp();
    }

    Keys.onVolumeDownPressed: {
        xbmc.volumeDown();
    }

    Loader {
        anchors.fill: parent
        sourceComponent: xbmc.connected ? mainComponent : noConnectionComponent
    }

    Connections {
        target: xbmc
        onConnectingChanged: print("connecting.....", xbmc.connecting)

        onAuthenticationRequired: {
            print("auth required");
            PopupUtils.open(authComponent, appWindow, {hostname: hostname})
        }

/*        onDisplayNotification: {
            print("************ notification", text, notificationBanner)
            notificationBanner.text = text;
            notificationBanner.show();
        }
        */
    }

    Connections {
        target: xbmc.keys()
        onInputRequested: {
            if (type === "date") {
                var year = value.split("/")[2]
                var month = value.split("/")[1] - 1
                var day = value.split("/")[0]
                var p = PopupUtils.open(datePicker, appWindow, {day: day, month: month, year: year});
                appWindow.inputDialog = p;
                return;
            }

            if (type === "time") {
                var p = PopupUtils.open(timePicker, appWindow);
                p.hour = value.split(":")[0]
                p.minute = value.split(":")[1]
                appWindow.inputDialog = p;
                return;
            }

            appWindow.inputDialog = PopupUtils.open(inputSheetComponent, appWindow);
            if (type === "number" || type === "numericpassword" || type === "seconds") {
                appWindow.inputDialog.inputMethodHints = Qt.ImhDigitsOnly
            }
            appWindow.inputDialog.description = title;
            appWindow.inputDialog.initialValue = value;
        }
        onInputFinished: PopupUtils.close(appWindow.inputDialog);
    }

    Component {
        id: noConnectionComponent
        Page {
            id: noConnectionPage
            title: xbmc.connecting ? qsTr("Connecting...") : qsTr("Select Host")
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
                        xbmc.hostModel().host(index).connect()
                    }

                    onPressAndHold: {
                        var obj = PopupUtils.open(removePopoverComponent, hostDelegate)
                        obj.removeClicked.connect(function() {
                            xbmc.hostModel().removeHost(index)
                            PopupUtils.close(obj)
                        })
                        obj.wakeupClicked.connect(function() {
                            xbmc.hostModel().host(index).wakeup();
                            PopupUtils.close(obj)
                        })
                        obj.editClicked.connect(function() {
                            PopupUtils.open(addHostComponent, noConnectionPage, {host: xbmc.hostModel().host(index)});
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
                        fontSize: "small"
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
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: xbmc.connectionError
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
                Label {
                    fontSize: "small"
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: qsTr("Please enable the following options in the Services settings of XBMC:") + "\n- "
                          + qsTr("Allow control of XBMC via HTTP") + "\n- "
                          + qsTr("Allow programs on other systems to control XBMC")
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
                    action: Action {
                        text: "add"
                        iconName: "add"
                        visible: noConnectionPage.showList
                        onTriggered: {
                            var newHost = newHostComponent.createObject();
                            var popup = PopupUtils.open(addHostComponent, noConnectionPage, {host: newHost});
                            popup.rejected.connect(function() {
                                newHost.destroy();
                            })
                            popup.accepted.connect(function() {
                                xbmc.hostModel().addHost(newHost);
                            })
                        }

                    }
                    Component {
                        id: newHostComponent
                        XbmcHost {}
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
            signal editClicked()
            Column {
                height: childrenRect.height
                width: parent.width
                ListItems.Standard {
                    text: qsTr("Remove")
                    iconName: "delete"
                    onClicked: popover.removeClicked()
                }
                ListItems.Standard {
                    text: qsTr("Wake up")
                    iconName: "torch-on"
                    onClicked: popover.wakeupClicked()
                }
                ListItems.Standard {
                    text: qsTr("Edit")
                    iconName: "edit"
                    onClicked: popover.editClicked();
                }
            }
        }
    }

    Component {
        id: addHostComponent
        Dialog {
            id: addHostDialog
            title: qsTr("Add host")

            property var host

            signal accepted();
            signal rejected();

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
                            text: addHostDialog.host.hostname
                            property bool conflicting: false

                            onTextChanged: {
                                print("model count", xbmc.hostModel().count)
                                for (var i = 0; i < xbmc.hostModel().count; ++i) {
                                    print("got:", xbmc.hostModel().get(i, "name"))
                                    if (xbmc.hostModel().get(i, "name") == text) {
                                        conflicting = true;
                                        return;
                                    }
                                    conflicting = false;
                                }
                            }

                            states: [
                                State {
                                    name: "conflicting"; when: nameTextField.conflicting
                                    PropertyChanges {
                                        target: nameTextField
                                        color: "red"
                                    }
                                }

                            ]
                        }
                        Label {
                            text: qsTr("Hostname or IP Address:")
                        }
                        TextField {
                            id: addressTextField
                            width: parent.width
                            text: addHostDialog.host.address
                        }
                        Label {
                            text: qsTr("Port:")
                        }
                        TextField {
                            id: portTextField
                            text: addHostDialog.host.port ? addHostDialog.host.port : "8080"
                            width: parent.width
                        }
                        Label {
                            text: qsTr("Mac Address:")
                        }
                        TextField {
                            id: macTextField
                            width: parent.width
                            inputMask: "HH:HH:HH:HH:HH:HH;_"
                            text: addHostDialog.host.hwAddr
                        }
                        Row {
                            width: parent.width
                            spacing: units.gu(1)
                            Button {
                                text: qsTr("Cancel")
                                width: (parent.width - parent.spacing) / 2
                                onClicked: {
                                    addHostDialog.rejected();
                                    PopupUtils.close(addHostDialog)
                                }
                            }
                            Button {
                                text: qsTr("OK")
                                width: (parent.width - parent.spacing) / 2
                                color: "#dd4814"
                                enabled: nameTextField.text.length > 0 && !nameTextField.conflicting && addressTextField.text.length > 0
                                onClicked: {
                                    addHostDialog.host.hostname = nameTextField.text
                                    addHostDialog.host.address = addressTextField.text
                                    addHostDialog.host.port = portTextField.text
                                    addHostDialog.host.hwAddr = macTextField.text

                                    addHostDialog.accepted();
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
        PageStack {
            id: pageStack
            Component.onCompleted: push(mainPage)

            MainPage {
                id: mainPage
                visible: false

                keypadVisible: true
                nowPlayingVisible: true

                onGoToNowPlaying: pageStack.push(nowPlayingPage)
                onGoToKeypad: pageStack.push(keypad)
            }
            NowPlayingPage {
                id: nowPlayingPage
                timerActive: pageStack.currentPage == nowPlayingPage
                visible: false

                keypadVisible: true

                onGoToKeypad: {
                    pageStack.pop()
                    pageStack.push(keypad)
                }
            }
            Keypad {
                id: keypad
                visible: false

                nowPlayingVisible: true

                onGoToNowPlaying: {
                    pageStack.pop();
                    pageStack.push(nowPlayingPage)
                }
            }
        }
    }

    Component {
        id: authComponent

        Dialog {
            id: authDialog
//            title: qsTr("Add host")
//            title: hostname
//            title: qsTr("XBMC on %1 requires authentication:").arg(hostname);

            property string hostname

            Flickable {
                id: flickbl
                width: parent.width
                height: units.gu(60)
                contentHeight: authColumn.height
                interactive: contentHeight > height
                Column {
                    id: authColumn
                    width: parent.width
                    spacing: units.gu(1)
                    Label {
                        width: parent.width
                        text: qsTr("XBMC on %1 requires authentication:").arg(hostname);
                        wrapMode: Text.WordWrap
                    }
                    Label {
                        text: qsTr("Username")
                    }
                    TextField {
                        width: parent.width
                        id: username
                    }
                    Label {
                        text: qsTr("Password")
                    }
                    TextField {
                        width: parent.width
                        id: password
                    }
                    Row {
                        width: parent.width
                        spacing: units.gu(1)

                        Button {
                            text: qsTr("Cancel")
                            width: (parent.width - parent.spacing) / 2
                            onClicked: PopupUtils.close(authDialog)
                        }
                        Button {
                            text: qsTr("OK")
                            width: (parent.width - parent.spacing) / 2
                            onClicked: {
                                xbmc.setAuthCredentials(username.text, password.text);
                                PopupUtils.close(authDialog)
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

    Component {
        id: datePicker
        DatePicker {
            onAccepted: {
                var date = new Date(year, month, day);
                var dateString = Qt.formatDate(date, "dd/MM/yyyy");
                print("Sending text", dateString);
                xbmc.keys().sendText(dateString);
            }
            onRejected: {
                xbmc.keys().previousMenu();
            }
        }
    }

    Component {
        id: timePicker
        TimePicker {
            onAccepted: {
                var date = new Date(0, 0, 0, hour, minute);
                var timeString = Qt.formatTime(date, "hh:mm");
                print("Sending text", timeString);
                xbmc.keys().sendText(timeString);
            }
            onRejected: {
                xbmc.keys().previousMenu();
            }
        }
    }

    Component {
        id: inputSheetComponent
        ComposerSheet {
            id: inputSheet
            property alias description: descriptionLabel.text
            property alias initialValue: inputField.text
            property alias inputMethodHints: inputField.inputMethodHints


            Component.onCompleted: {
                inputField.forceActiveFocus();
                __foreground.__styleInstance.headerColor = "#0a2663"
                __foreground.__styleInstance.backgroundColor = "#1b62c8"
            }

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20
                Label {
                    id: descriptionLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                TextField {
                    width: parent.width
                    id: inputField
                }
            }
            onConfirmClicked: {
                xbmc.keys().sendText(inputField.text);
            }
            onCancelClicked: {
                xbmc.keys().previousMenu();
            }
        }
    }

}
