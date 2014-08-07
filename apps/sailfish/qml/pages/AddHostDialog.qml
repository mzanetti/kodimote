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

Dialog {
    id: addHostDialog
    property QtObject host

    canAccept: nameTextField.text.length > 0 && addressTextField.text.length > 0 && portTextField.text.length > 0

    Column {
        anchors.fill: parent

        DialogHeader {
            acceptText: qsTr("Add")
        }

        TextField {
            id: nameTextField
            width: parent.width
            text: host.hostname

            placeholderText: qsTr("Name")
            label: qsTr("Name")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: addressTextField.focus = true

            property bool conflicting: false

            onTextChanged: {
                for (var i = 0; i < xbmc.hostModel().count; ++i) {
                    if (xbmc.hostModel().get(i, "name") == text && xbmc.hostModel().getHost(i) != host) {
                        conflicting = true;
                        return;
                    }
                    conflicting = false;
                }
            }

            states: [
                State {
                    name: "conflicting";
                    when: nameTextField.conflicting
                    PropertyChanges {
                        target: nameTextField
                        color: "red"
                    }
                }
            ]
        }

        TextField {
            id: addressTextField
            width: parent.width
            text: host.address

            placeholderText: qsTr("Hostname or IP Address")
            label: qsTr("Hostname or IP Address")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: portTextField.focus = true
        }

        TextField {
            id: portTextField
            width: parent.width
            text: host.port

            placeholderText: qsTr("Port")
            label: qsTr("Port")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: macTextField.focus = true
        }

        TextField {
            id: macTextField
            width: parent.width
            text: host.hwAddr

            placeholderText: qsTr("Mac Address")
            label: qsTr("Mac Address")

            EnterKey.enabled: addHostDialog.canAccept
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: addHostDialog.accept()
        }
    }

    onAccepted: {
        host.hostname = nameTextField.text
        host.address = addressTextField.text
        host.port = portTextField.text
        host.hwAddr = macTextField.text
        xbmc.hostModel().insertOrUpdateHost(host);
    }
}
