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
import QtFeedback 5.0
import harbour.xbmcremote 1.0
import "../components/"

Page {
    id: keypad

    property QtObject picturePlayer: xbmc.picturePlayer()

    property bool usePictureControls: xbmc.picturePlayerActive && !pictureControlsOverride
    property bool pictureControlsOverride: false

    property QtObject keys: xbmc.keys()

    HapticsEffect {
        id: rumbleEffect
        attackIntensity: 0
        attackTime: 250
        intensity: 1.0
        fadeTime: 250
        fadeIntensity: 0
        period: 100
    }

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Keypad")
        }

        PullDownMenu {
            MenuPlayerControls {

            }

            MenuItem {
                text: qsTr("Media")
                onClicked: {
                    pageStack.pop();
                }
            }

            MenuItem {
                text: qsTr("Keypad")
                enabled: xbmc.picturePlayerActive

            }

            MenuItem {
                text: qsTr("Now playing")
                enabled: xbmc.activePlayer !== null
                onClicked: {
                    pageStack.replace("NowPlayingPage.qml")
                }
            }
        }

        PushUpMenu {
            visible: xbmc.picturePlayerActive
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
            anchors.margins: Theme.paddingLarge

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
                        onClicked: xbmc.switchToWindow(Xbmc.GuiWindowPictures)
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-music"
                        onClicked: xbmc.switchToWindow(Xbmc.GuiWindowMusic)
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-home"
                        onClicked: keys.home()
                    }
                    IconButton {
                        icon.source: "image://theme/icon-m-video"
                        onClicked: xbmc.switchToWindow(Xbmc.GuiWindowVideos)
                    }
                    IconButton {
                        icon.source: "../icons/icon-m-tv.png"
                        onClicked: xbmc.switchToWindow(Xbmc.GuiWindowLiveTV)
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
                        rumbleEffect.start(2);
                        keys.back();
                    }
                }
                IconButton {
                    icon.source: usePictureControls ? "image://theme/icon-m-add" : "image://theme/icon-m-back"
                    rotation: usePictureControls ? 0 : 135
                    anchors { right: parent.right; top: parent.top; margins: Theme.paddingMedium }
                    onClicked: {
                        rumbleEffect.start(2);
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
                        rumbleEffect.start(2);
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
                        rumbleEffect.start(2);
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

                PlayerControls {
                    anchors.centerIn: parent
                    player: usePictureControls ? keypad.picturePlayer : xbmc.activePlayer
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
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { rumbleEffect.start(2); keys.red() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { rumbleEffect.start(2); keys.green() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { rumbleEffect.start(2); keys.yellow() } }
                    }
                    Rectangle { height: 20; width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea { anchors.fill: parent; anchors.margins: -10; onClicked: { rumbleEffect.start(2); keys.blue() } }
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
