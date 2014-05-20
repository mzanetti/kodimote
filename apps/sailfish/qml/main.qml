/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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
import "pages"

ApplicationWindow
{
    id: appWindow

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    initialPage: mainPageComponent

    Component {
        id: mainPageComponent
        MainPage {

        }
    }

    Keys.onVolumeDownPressed: {
        xbmc.volume -= 5;
    }

    Keys.onVolumeUpPressed: {
        xbmc.volume += 5;
    }

    function showAuthenticate(hostname) {
        var component = Qt.createComponent("pages/AuthenticationDialog.qml")
        if (component.status == Component.Ready) {
            var authDialog = component.createObject(initialPage);
            authDialog.hostname = hostname;
            authDialog.open();
        } else {
            console.log("Error loading component:", component.errorString());
        }
    }

    Timer {
        /*Hack, we can't use pageStack.push while the pageStack is animating*/
        id: delayedAuthenticate
        interval: 10
        repeat: true
        running: false
        onTriggered: {
            if (pageStack.busy) {
                return;
            }

            stop();
            showAuthenticate(hostname)
        }
        property string hostname
    }

    Connections {
        target: xbmc
        onAuthenticationRequired: {
            if (pageStack.busy) {
                delayedAuthenticate.hostname = hostname;
                delayedAuthenticate.start();
            } else {
                showAuthenticate(hostname);
            }
        }
    }

    Component {
        id: datePickerComponent
        DatePickerDialog {
            property string initialValue
            readonly property string value: Qt.formatDate(date, "dd/MM/yyyy")

            onInitialValueChanged: {
                var splitted = initialValue.split("/");
                date = new Date(splitted[2], splitted[1], splitted[0]);
            }
        }
    }

    Component {
        id: timePickerComponent
        TimePickerDialog {
            property string initialValue
            readonly property string value: Qt.formatTime(time, "hh:mm")

            onInitialValueChanged: {
                var splitted = initialValue.split(":");
                hour = splitted[0];
                minute = splitted[1];
            }
        }
    }

    Component {
        id: inputComponent

        Dialog {
            property alias initialValue: inputField.text
            property alias value: inputField.text

            property alias title: inputField.label
            property alias inputMethodHints: inputField.inputMethodHints

            Component.onCompleted: inputField.forceActiveFocus()

            DialogHeader {
                id: header
            }

            TextField {
                id: inputField
                anchors.top: header.bottom
                width: parent.width

                placeholderText: label

                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: parent.accept()
            }
        }
    }

    property variant inputDialog

    Connections {
        target: xbmc.keys()
        onInputRequested: {
            if (type === "date") {
                appWindow.inputDialog = datePickerComponent.createObject(appWindow);
            } else if (type === "time") {
                appWindow.inputDialog = timePickerComponent.createObject(appWindow);
            } else {
                appWindow.inputDialog = inputComponent.createObject(appWindow);
                appWindow.inputDialog.title = title;

                if (type === "number" || type === "numericpassword" || type === "seconds") {
                    appWindow.inputDialog.inputMethodHints = Qt.ImhDigitsOnly;
                }
            }

            appWindow.inputDialog.initialValue = value;
            appWindow.inputDialog.accepted.connect(function() {
                var value = appWindow.inputDialog.value;
                console.log("Sending text: " + value)
                xbmc.keys().sendText(value);
            });
            appWindow.inputDialog.rejected.connect(function() {
                xbmc.keys().previousMenu();
            });
            appWindow.inputDialog.open();
        }
        onInputFinished: {
            if (appWindow.inputDialog) {
                appWindow.inputDialog.close();
                appWindow.inputDialog = null;
            }
        }
    }
}
