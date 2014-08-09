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
import Ubuntu.Components 0.1
import Xbmc 1.0
import "components"

XbmcPage {
    id: root
    title: qsTr("Keypad")

    property QtObject player: xbmc.activePlayer
    property QtObject picturePlayer: xbmc.picturePlayer()

    property bool usePictureControls: xbmc.picturePlayerActive && !pictureControlsOverride
    property bool pictureControlsOverride: false

    property int spacing: units.gu(2)

    function teaseArrows() {
        gesturePad.teaseArrows();
    }

    property QtObject keys: xbmc.keys()

    Column {
        anchors {
            left: parent.left
            right: parent.right
            margins: root.spacing
            verticalCenter: parent.verticalCenter
        }
        spacing: root.spacing

        UbuntuShape {
            width: parent.width
            height: musicButton.height + root.spacing
            color: Qt.rgba(0, 0, 0, 0.05)

            Row {
                id: jumpPointRow
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: root.spacing / 2
                    verticalCenter: parent.verticalCenter
                }
                height: musicButton.height

                spacing: (width - musicButton.width * 5) / 4

                MediaControlButton {
                    source: "image://theme/gallery-symbolic"
                    onClicked: xbmc.switchToWindow(Xbmc.GuiWindowPictures);
                }
                MediaControlButton {
                    id: musicButton
                    source: "image://theme/stock_music"
                    onClicked: xbmc.switchToWindow(Xbmc.GuiWindowMusic);
                }
                MediaControlButton {
                    source: "image://theme/go-home"
                    onClicked: keys.home();
                }
                MediaControlButton {
                    source: "image://theme/stock_video"
                    onClicked: xbmc.switchToWindow(Xbmc.GuiWindowVideos);

                }
                MediaControlButton {
                    source: "../images/livetv.svg"
                    onClicked: xbmc.switchToWindow(Xbmc.GuiWindowLiveTV);
                }
            }
        }

        GesturePad {
            id: gesturePad
            width: parent.width
            height: width * 3 / 4

            MediaControlButton {
                id: backButton
                anchors { left: parent.left; top: parent.top; margins: units.gu(1.5) }
                source: "image://theme/back"
                AbstractButton {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: 55
                    onClicked: {
                        keys.back();
                    }
                }

            }

            MediaControlButton {
                anchors { right: parent.right; top: parent.top; margins: units.gu(1.5) }
                source: usePictureControls ? "image://theme/add" : "image://theme/view-fullscreen"
                AbstractButton {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: usePictureControls ? -145 : -190
                    onClicked: {
                        if (usePictureControls) {
                            picturePlayer.zoomIn();
                        } else {
                            keys.fullscreen();
                        }
                    }
                }
            }
            MediaControlButton {
                anchors { left: parent.left; bottom: parent.bottom; margins: units.gu(1.5) }
                source: usePictureControls ? "image://theme/reload" : "image://theme/info"
                AbstractButton {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: -145
                    onClicked: {
                        if (usePictureControls) {
                            picturePlayer.rotate();
                        } else {
                            keys.info();
                        }
                    }
                }
            }
            MediaControlButton {
                anchors { right: parent.right; bottom: parent.bottom; margins: units.gu(1.5) }
                source: usePictureControls ? "image://theme/remove" : "image://theme/navigation-menu"
                AbstractButton {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    onClicked: {
                        if (usePictureControls) {
                            picturePlayer.zoomOut();
                        } else {
                            keys.osd();
                            keys.contextMenu();
                        }
                    }
                    rotation: 145
                }
            }
        }

        UbuntuShape {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: controlButtons.height + root.spacing
            color: Qt.rgba(0, 0, 0, 0.05)

            PlayerControls {
                id: controlButtons
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: root.spacing / 2
                    verticalCenter: parent.verticalCenter
                }

                player: usePictureControls ? root.picturePlayer : root.player
            }
        }

        UbuntuShape {
            width: parent.width
            height: colorButtonRow.height + root.spacing
            color: Qt.rgba(0, 0, 0, 0.05)

            Row {
                id: colorButtonRow
                width: parent.width - spacing
                anchors.centerIn: parent
                height: controlButtons.height
                spacing: parent.width / 8
                UbuntuShape { height: units.gu(2); width: parent.spacing; color: "red"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton { anchors.fill: parent; anchors.margins: -10; onClicked: { keys.red() } }
                }
                UbuntuShape { height: units.gu(2); width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton { anchors.fill: parent; anchors.margins: -10; onClicked: { keys.green() } }
                }
                UbuntuShape { height: units.gu(2); width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton { anchors.fill: parent; anchors.margins: -10; onClicked: { keys.yellow() } }
                }
                UbuntuShape { height: units.gu(2); width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton { anchors.fill: parent; anchors.margins: -10; onClicked: { keys.blue() } }
                }
            }
        }
    }

    BottomEdge {
        BottomEdgeButton {
            text: usePictureControls ? "Keypad" : "Pictures"
            source: usePictureControls ? "image://theme/keypad" : "image://theme/gallery-symbolic"
            visible: xbmc.picturePlayerActive
            onClicked: {
                pictureControlsOverride = !pictureControlsOverride;
            }
        }
    }
}
