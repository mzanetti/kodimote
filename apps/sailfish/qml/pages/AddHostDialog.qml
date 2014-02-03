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

Dialog {
    canAccept: nameTextField.text && addressTextField.text && portTextField.text

    Column {
        anchors.margins: Theme.paddingMedium
        anchors.fill: parent

        DialogHeader {
            acceptText: qsTr("Add")
        }

        Label {
            text: qsTr("Name:")
        }
        TextField {
            id: nameTextField
            width: parent.width
            property bool conflicting: false

            onTextChanged: {
                for (var i = 0; i < xbmc.hostModel().count; ++i) {
                    if (xbmc.hostModel().get(i, "name") == text) {
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
        }
    }

    onAccepted: {
        xbmc.hostModel().createHost(nameTextField.text, addressTextField.text, portTextField.text, macTextField.text);
    }
}
