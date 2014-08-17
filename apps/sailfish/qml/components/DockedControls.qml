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
import harbour.xbmcremote 1.0

DockedPanel {
    id: panel
    property QtObject player: xbmc.activePlayer
    property bool hideTemporary: false
    property bool _opened

    open: player
    width: parent.width
    height: column.height + (2 * Theme.paddingLarge)
    contentHeight: height

    onHideTemporaryChanged: {
        if (hideTemporary) {
            _opened = open;
            hide(true);
        } else {
            if (_opened) {
                show(true);
            }
        }
    }

    Connections {
        target: Qt.inputMethod
        onVisibleChanged: {
            if (Qt.inputMethod.visible) {
                panel.hide(true);
            } else {
                panel.show(true);
            }
        }
    }

    Column {
        id: column
        width:parent.width
        height: childrenRect.height
        anchors.verticalCenter: parent.verticalCenter
        spacing: Theme.paddingMedium

        Item {
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: Theme.paddingLarge
                rightMargin: Theme.paddingLarge
            }

            height: 64

            IconButton {
                id: volumeDownButton
                height: 64
                width: height
                anchors.left: parent.left
                icon.source: "../icons/icon-m-volume-down.png"
                onClicked: xbmc.volumeDown()
            }

            Slider {
                id: volumeSlider
                anchors.left: volumeDownButton.right
                anchors.right: volumeUpButton.left
                enabled: xbmc.connectedHost.volumeControlType !== XbmcHost.VolumeControlTypeRelative
                visible: enabled
                leftMargin: Theme.paddingSmall
                rightMargin: Theme.paddingLarge

                minimumValue: 0
                maximumValue: 100

                Binding {
                    target: volumeSlider
                    property: "value"
                    value: xbmc.volume
                }
            }

            IconButton {
                id: volumeUpButton
                height: 64
                width: height
                anchors.right: parent.right
                icon.source: "image://theme/icon-m-speaker"
                onClicked: xbmc.volumeUp()
            }
        }

        PlayerControls {
            anchors.horizontalCenter: parent.horizontalCenter
            player: panel.player
        }
    }
}
