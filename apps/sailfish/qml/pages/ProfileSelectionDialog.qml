/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
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
    id: dialog

    property string lockCode

    onAccepted: {
        listView.model.switchProfile(listView.currentIndex, dialog.lockCode)
    }

    Connections {
        target: listView.model
        onCurrentProfileIndexChanged: {
            if (listView.currentIndex === -1) {
                listView.currentIndex = listView.model.currentProfileIndex;
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: listView.model.busy
        visible: listView.model.busy
        size: BusyIndicatorSize.Large
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: kodi.profiles();
        currentIndex: model.currentProfileIndex

        header: DialogHeader {
            acceptText: qsTr("Select user")
        }

        VerticalScrollDecorator {}

        delegate: ListItem {
            id: item
            property bool active: listView.currentIndex === index
            property bool mustUnlock: lockMode === KodiModel.LockModeNumeric || lockMode === KodiModel.LockModeAlphaNumeric;

            width: listView.width
            height: active && mustUnlock ? Theme.itemSizeExtraLarge : Theme.itemSizeLarge
            contentHeight: height

            highlighted: down || active

            onClicked: {
                listView.currentIndex = index
            }

            onActiveChanged: {
                if (active) {
                    if (mustUnlock) {
                        lockCode.forceActiveFocus();
                        dialog.canAccept = Qt.binding(function() { return lockCode.text.length > 0; });
                    } else {
                        dialog.canAccept = true;
                    }
                }
            }

            Behavior on height {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 300
                }
            }

            Column {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    text: title
                    anchors.margins: Theme.paddingLarge
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.weight: Font.Bold
                    font.pixelSize: Theme.fontSizeMedium
                    truncationMode: TruncationMode.Fade
                }

                Item {
                    width: parent.width
                    clip: true
                    visible: mustUnlock && (item.active || animation.running)
                    height: item.active ? lockCode.height : 0

                    Behavior on height {
                        NumberAnimation {
                            id: animation
                            easing.type: Easing.OutQuad
                            duration: 300
                        }
                    }

                    TextField {
                        id: lockCode
                        width: parent.width
                        label: qsTr("Lock code")
                        placeholderText: qsTr("Lock code")
                        echoMode: TextInput.Password
                        inputMethodHints: (lockMode === KodiModel.LockModeNumeric ? Qt.ImhDigitsOnly : 0) | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
                        onTextChanged: {
                            dialog.lockCode = text;
                        }

                        EnterKey.enabled: text.length > 0
                        EnterKey.iconSource: "image://theme/icon-m-enter-next"
                        EnterKey.onClicked: dialog.accept()
                    }
                }
            }
        }
    }
}
