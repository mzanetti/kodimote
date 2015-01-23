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
import QtFeedback 5.0
import harbour.kodimote 1.0
import "../components/"

Page {
    id: keypad

    property QtObject picturePlayer: kodi.picturePlayer()

    property bool usePictureControls: kodi.picturePlayerActive && !pictureControlsOverride
    property bool pictureControlsOverride: false

    property QtObject keys: kodi.keys()

    HapticsEffect {
        id: rumbleEffect
        intensity: 0.50
        duration: 50
    }

    SilicaFlickable {
        anchors.fill: parent
        pressDelay: 0

        PageHeader {
            id: header
            title: qsTr("Keypad")
        }

        PullDownMenu {
            ControlsMenuItem {

            }

            MenuItem {
                text: qsTr("Media")
                onClicked: {
                    pageStack.pop();
                }
            }

            MenuItem {
                text: qsTr("Keypad")
                enabled: kodi.picturePlayerActive

            }

            MenuItem {
                text: qsTr("Now playing")
                enabled: kodi.activePlayer !== null
                onClicked: {
                    pageStack.replace("NowPlayingPage.qml")
                }
            }
        }

        PushUpMenu {
            enabled: kodi.picturePlayerActive
            visible: kodi.picturePlayerActive
            MenuItem {
                text: !enabled || usePictureControls ? qsTr("Keypad") : qsTr("Pictures")
                onClicked: {
                    pictureControlsOverride = !pictureControlsOverride
                }
            }
        }

        contentHeight: column.childrenRect.height

        Column {
            id: column

            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge

            spacing: Theme.paddingLarge * 1.5

            Rectangle {
                width: parent.width
                color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                height: Theme.itemSizeMedium

                Row {
                    anchors.centerIn: parent
                    spacing: Theme.paddingMedium

                    IconButton {
                        icon.source: "image://theme/icon-m-image"
                        onClicked: {
                            if (settings.hapticsEnabled) {
                                rumbleEffect.start(2);
                            }

                            kodi.switchToWindow(Kodi.GuiWindowPictures)
                        }
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-music"
                        onClicked: {
                            if (settings.hapticsEnabled) {
                                rumbleEffect.start(2);
                            }

                            kodi.switchToWindow(Kodi.GuiWindowMusic)
                        }
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-home"
                        onClicked: {
                            if (settings.hapticsEnabled) {
                                rumbleEffect.start(2);
                            }

                            keys.home()
                        }
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-video"
                        onClicked: {
                            if (settings.hapticsEnabled) {
                                rumbleEffect.start(2);
                            }

                            kodi.switchToWindow(Kodi.GuiWindowVideos)
                        }
                    }
                    IconButton {
                        icon.source: "../icons/icon-m-tv.png"
                        onClicked: {
                            if (settings.hapticsEnabled) {
                                rumbleEffect.start(2);
                            }

                            kodi.switchToWindow(Kodi.GuiWindowLiveTV)
                        }
                    }
                }
            }

            GesturePad {
                id: gesturePad
                width: parent.width

                IconButton {
                    icon.source: "image://theme/icon-m-back"
                    anchors { left: parent.left; top: parent.top; margins: Theme.paddingMedium }
                    onClicked: {
                        if (settings.hapticsEnabled) {
                            rumbleEffect.start(2);
                        }

                        keys.back();
                    }
                }
                IconButton {
                    icon.source: usePictureControls ? "image://theme/icon-m-add" : "image://theme/icon-m-back"
                    rotation: usePictureControls ? 0 : 135
                    anchors { right: parent.right; top: parent.top; margins: Theme.paddingMedium }
                    onClicked: {
                        if (settings.hapticsEnabled) {
                            rumbleEffect.start(2);
                        }

                        if (usePictureControls) {
                            picturePlayer.zoomIn();
                        } else {
                            keys.fullscreen();
                        }
                    }
                }
                IconButton {
                    icon.source: usePictureControls ? "image://theme/icon-m-refresh" : "image://theme/icon-m-about"
                    anchors { left: parent.left; bottom: parent.bottom; margins: Theme.paddingMedium }
                    onClicked: {
                        if (settings.hapticsEnabled) {
                            rumbleEffect.start(2);
                        }

                        if (usePictureControls) {
                            picturePlayer.rotate();
                        } else {
                            keys.info();
                        }
                    }
                }
                IconButton {
                    icon.source: usePictureControls ? "image://theme/icon-m-remove" : "../icons/icon-m-menu.png"
                    anchors { right: parent.right; bottom: parent.bottom; margins: Theme.paddingMedium }
                    onClicked: {
                        if (settings.hapticsEnabled) {
                            rumbleEffect.start(2);
                        }

                        if (usePictureControls) {
                            picturePlayer.zoomOut();
                        } else {
                            keys.osd();
                            keys.contextMenu();
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                height: Theme.itemSizeMedium

                Row {
                    anchors.centerIn: parent
                    spacing: parent.width / 8
                    Rectangle { height: 20; width: parent.spacing; color: "red"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { if (settings.hapticsEnabled) { rumbleEffect.start(2); } keys.red() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { if (settings.hapticsEnabled) { rumbleEffect.start(2); } keys.green() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { if (settings.hapticsEnabled) { rumbleEffect.start(2); } keys.yellow() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { if (settings.hapticsEnabled) { rumbleEffect.start(2); } keys.blue() } }
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            gesturePad.teaseArrows();
        }
    }
}
