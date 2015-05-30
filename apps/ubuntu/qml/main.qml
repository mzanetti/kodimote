/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 2.0
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItems
import Kodi 1.0
import "components"
import Ubuntu.Content 1.1

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
        kodi.volumeUp();
    }

    Keys.onVolumeDownPressed: {
        kodi.volumeDown();
    }

    Connections {
        target: ContentHub

        onShareRequested: {
            var filePath = transfer.items[0].url
            print("Should share link", filePath)
//            uploader.uploadImage(filePath);

            protocolManager.execute(filePath)
        }
    }

    Binding {
        target: kodi
        property: "active"
        value: Qt.application.active
    }

    Loader {
        anchors.fill: parent
        sourceComponent: kodi.connected ? mainComponent : noConnectionComponent
    }

    Connections {
        target: kodi
        onConnectingChanged: print("connecting.....", kodi.connecting)

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
        target: kodi.keys()
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
            title: kodi.connecting ? qsTr("Connecting...") : qsTr("Select Host")
            anchors.fill: parent
            property bool showList: !kodi.connecting

            ListView {
                id: hostListView
                anchors.fill: parent
                model: kodi.hostModel()
                opacity: noConnectionPage.showList ? 1 : 0
                Behavior on opacity { UbuntuNumberAnimation {} }

                delegate: ListItemWithActions {
                    id: hostDelegate
                    width: parent.width
                    height: units.gu(8)
                    color: "transparent"
                    triggerActionOnMouseRelease: true

                    leftSideAction: Action {
                        iconName: "delete"
                        onTriggered: kodi.hostModel().removeHost(index)
                    }
                    rightSideActions: [
                        Action {
                            iconName: "torch-on"
                            onTriggered: kodi.hostModel().host(index).wakeup();
                        },
                        Action {
                            iconName: "edit"
                            onTriggered: PopupUtils.open(addHostComponent, noConnectionPage, {host: kodi.hostModel().host(index)});
                        }

                    ]

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        text: hostname
                    }

                    onItemClicked: {
                        noConnectionPage.showList = false
                        kodi.hostModel().host(index).connect()
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
                        text: qsTr("Searching for Kodi hosts.") + "\n" + "\n"
                              + qsTr("Please enable the following options in the Services settings of Kodi:") + "\n- "
                              + qsTr("Allow control of Kodi via HTTP") + "\n- "
                              + qsTr("Allow programs on other systems to control Kodi") + "\n- "
                              + qsTr("Announce these services to other systems via Zeroconf") + "\n"
                              + qsTr("If you don't use Zeroconf, add a host manually.");
                    }
                }

                KodiDiscovery {
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
                    text: kodi.connectionError
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
                Label {
                    fontSize: "small"
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: qsTr("Please enable the following options in the Services settings of Kodi:") + "\n- "
                          + qsTr("Allow control of Kodi via HTTP") + "\n- "
                          + qsTr("Allow programs on other systems to control Kodi")
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
                                popup.destroy();
                                newHost.destroy();
                            })
                            popup.accepted.connect(function() {
                                kodi.hostModel().addHost(newHost);
                            })
                        }

                    }
                    Component {
                        id: newHostComponent
                        KodiHost {}
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
            title: qsTr("Host settings")

            property var host

            Component.onCompleted: print("############# host", host)

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
                            color: "black"
                        }
                        TextField {
                            id: nameTextField
                            width: parent.width
                            text: addHostDialog.host.hostname
                            color: "black"
                            property bool conflicting: false

                            onTextChanged: {
                                print("model count", kodi.hostModel().count)
                                for (var i = 0; i < kodi.hostModel().count; ++i) {
                                    print("got:", kodi.hostModel().host(i).hostname)
                                    if (kodi.hostModel().host(i).hostname == text) {
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
                            color: "black"
                        }
                        TextField {
                            id: addressTextField
                            width: parent.width
                            text: addHostDialog.host.address
                            color: "black"
                        }
                        Label {
                            text: qsTr("Port:")
                            color: "black"
                        }
                        TextField {
                            id: portTextField
                            text: addHostDialog.host.port ? addHostDialog.host.port : "8080"
                            width: parent.width
                            color: "black"
                        }
                        Label {
                            text: qsTr("Mac Address:")
                            color: "black"
                        }
                        TextField {
                            id: macTextField
                            width: parent.width
                            inputMask: "HH:HH:HH:HH:HH:HH;_"
                            color: "black"
                            text: addHostDialog.host.hwAddr
                        }

                        SectionHeader {
                            headerText: qsTr("Volume")
                            color: "black"
                        }

                        OptionSelector {
                            id: volumeControlTypeSelector
                            model: [qsTr("Custom Stepping"), qsTr("Up or down"), qsTr("Custom script")]
                            selectedIndex: addHostDialog.host.volumeControlType

                            // Hack... OptionSelector doesn't let us set the color and uitk is buggy with colors atm
                            delegate: OptionSelectorDelegate {
                                text: " "
                                Label {
                                    anchors { left: parent.left; verticalCenter: parent.verticalCenter; leftMargin: units.gu(2) }
                                    text: modelData
                                    color: "black"
                                }
                            }
                        }

                        RowLayout {
                            visible: volumeControlTypeSelector.selectedIndex !== 1
                            Label { text: "0"; color: "black" }
                            anchors { left: parent.left; right: parent.right }
                            spacing: units.gu(1)
                            Slider {
                                id: volumeSteppingSlider
                                value: addHostDialog.host.volumeStepping
                                Layout.fillWidth: true
                            }
                            Label { text: "100"; color: "black" }
                        }

                        Label {
                            text: qsTr("Up command"); color: "black";
                            visible: volumeControlTypeSelector.selectedIndex === 2
                        }
                        TextField {
                            id: volumeUpCommandTextField
                            width: parent.width
                            visible: volumeControlTypeSelector.selectedIndex === 2
                            text: host.volumeUpCommand
                            placeholderText: qsTr("Up command")
                            color: "black"
                        }

                        Label {
                            text: qsTr("Down command"); color: "black"
                            visible: volumeControlTypeSelector.selectedIndex === 2
                        }
                        TextField {
                            id: volumeDownCommandTextField
                            width: parent.width
                            visible: volumeControlTypeSelector.selectedIndex === 2
                            text: host.volumeDownCommand
                            placeholderText: qsTr("Down command")
                            color: "black"
                        }

                        SectionHeader {
                            color: "black"
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

                                    addHostDialog.host.volumeControlType = volumeControlTypeSelector.selectedIndex
                                    addHostDialog.host.volumeStepping = volumeSteppingSlider.value
                                    addHostDialog.host.volumeUpCommand = volumeUpCommandTextField.text
                                    addHostDialog.host.volumeDownCommand = volumeDownCommandTextField.text
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

                mediaVisible: true
                keypadVisible: true

                onGoToKeypad: {
                    pageStack.pop()
                    pageStack.push(keypad)
                }
                onGoToMedia: pageStack.pop()
            }
            Keypad {
                id: keypad
                visible: false

                mediaVisible: true
                nowPlayingVisible: true

                onGoToNowPlaying: {
                    pageStack.pop();
                    pageStack.push(nowPlayingPage)
                }

                onGoToMedia: pageStack.pop()
            }
        }
    }

    Component {
        id: authComponent

        Dialog {
            id: authDialog
//            title: qsTr("Add host")
//            title: hostname
//            title: qsTr("Kodi on %1 requires authentication:").arg(hostname);

            property string hostname

            Flickable {
                id: flickbl
                width: parent.width
                height: contentHeight
                contentHeight: authColumn.height
                interactive: contentHeight > height
                Column {
                    id: authColumn
                    width: parent.width
                    spacing: units.gu(1)
                    Label {
                        width: parent.width
                        text: qsTr("Kodi on %1 requires authentication:").arg(hostname);
                        wrapMode: Text.WordWrap
                        color: "black"
                    }
                    Label {
                        text: qsTr("Username")
                        color: "black"
                    }
                    TextField {
                        width: parent.width
                        id: username
                    }
                    Label {
                        text: qsTr("Password")
                        color: "black"
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
                                kodi.setAuthCredentials(username.text, password.text);
                                PopupUtils.close(authDialog)
                            }
                        }
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
                kodi.keys().sendText(dateString);
            }
            onRejected: {
                kodi.keys().previousMenu();
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
                kodi.keys().sendText(timeString);
            }
            onRejected: {
                kodi.keys().previousMenu();
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
                kodi.keys().sendText(inputField.text);
            }
            onCancelClicked: {
                kodi.keys().previousMenu();
            }
        }
    }

}
