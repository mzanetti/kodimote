/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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
import Sailfish.Silica 1.0
import harbour.kodimote 1.0

Dialog {
    id: addHostDialog
    property QtObject host
    property alias title: dialogHeader.acceptText

    canAccept: nameTextField.text.length > 0 && addressTextField.text.length > 0 && portTextField.text.length > 0

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.childrenRect.height

        Column {
            id: column
            anchors.fill: parent

            DialogHeader {
                id: dialogHeader
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
                    for (var i = 0; i < kodi.hostModel().count; ++i) {
                        if (kodi.hostModel().host(i).hostname == text && kodi.hostModel().host(i) != host) {
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

            SectionHeader {
                text: qsTr("Volume")
            }

            ComboBox {
                id: volumeControlTypeComboBox
                width: parent.width
                currentIndex: host.volumeControlType

                label: qsTr("Control type")

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Custom stepping")
                    }
                    MenuItem {
                        text: qsTr("Up or down")
                    }
                    MenuItem {
                        text: qsTr("Custom script")
                    }
                }
            }

            Slider {
                id: volumeSteppingSlider
                width: parent.width
                visible: volumeControlTypeComboBox.currentIndex !== 1

                value: host.volumeStepping

                label: qsTr("Stepping")
                minimumValue: 1
                maximumValue: 25
                stepSize: 1
                valueText: value
            }

            TextField {
                id: volumeUpCommandTextField
                width: parent.width
                visible: volumeControlTypeComboBox.currentIndex === 2
                text: host.volumeUpCommand

                label: qsTr("Up command")
                placeholderText: qsTr("Up command")
            }

            TextField {
                id: volumeDownCommandTextField
                width: parent.width
                visible: volumeControlTypeComboBox.currentIndex === 2
                text: host.volumeDownCommand

                label: qsTr("Down command")
                placeholderText: qsTr("Down command")
            }
        }
    }

    onAccepted: {
        host.hostname = nameTextField.text
        host.address = addressTextField.text
        host.port = portTextField.text
        host.hwAddr = macTextField.text

        host.volumeControlType = volumeControlTypeComboBox.currentIndex
        host.volumeStepping = volumeSteppingSlider.value
        host.volumeUpCommand = volumeUpCommandTextField.text
        host.volumeDownCommand = volumeDownCommandTextField.text
    }
}
