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
import Ubuntu.Components 0.1
import Kodi 1.0
import "components"

KodiPage {
    id: root
    title: qsTr("Keypad")

    property QtObject player: kodi.activePlayer
    property QtObject picturePlayer: kodi.picturePlayer()

    property bool usePictureControls: kodi.picturePlayerActive && !pictureControlsOverride
    property bool pictureControlsOverride: false

    property int spacing: units.gu(2)

    function teaseArrows() {
        gesturePad.teaseArrows();
    }

    property QtObject keys: kodi.keys()

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

            Label {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                opacity: settings.introStep < Settings.IntroStepDone ? 1 : 0
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
                color: "white"
                text: {
                    switch (settings.introStep) {
                    case Settings.IntroStepLeftRight:
                        return qsTr("To move left or right, swipe horizontally.");
                    case Settings.IntroStepUpDown:
                        return qsTr("To move up or down, swipe vertically.");
                    case Settings.IntroStepScroll:
                        return qsTr("To scroll through lists, press and keep holding while dragging.");
                    case Settings.IntroStepClick:
                        return qsTr("To select an item, tap anywhere on the pad.");
                    case Settings.IntroStepColors:
                        return qsTr("Pro tip: The color buttons at the bottom simulate an infrared remote.")
                    }
                    return ""
                }
            }

            Row {
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
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
                    onClicked: kodi.switchToWindow(Kodi.GuiWindowPictures);
                }
                MediaControlButton {
                    id: musicButton
                    source: "image://theme/stock_music"
                    onClicked: kodi.switchToWindow(Kodi.GuiWindowMusic);
                }
                MediaControlButton {
                    source: "image://theme/go-home"
                    onClicked: keys.home();
                }
                MediaControlButton {
                    source: "image://theme/stock_video"
                    onClicked: kodi.switchToWindow(Kodi.GuiWindowVideos);

                }
                MediaControlButton {
                    source: "../images/livetv.svg"
                    onClicked: kodi.switchToWindow(Kodi.GuiWindowLiveTV);
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
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }

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
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
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
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
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
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
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

            Label {
                id: introLabel2
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "white"
                wrapMode: Text.WordWrap
                text: {
                    switch (settings.introStep) {
                    case Settings.IntroStepScroll:
                        return qsTr("The further you move, the faster you scroll.");
                    case Settings.IntroStepColors:
                        return qsTr("You can map them to anything you want in Kodi's Lircmap.xml")
                    }
                    return ""
                }
                opacity: (settings.introStep == Settings.IntroStepScroll || settings.introStep == Settings.IntroStepColors) ? 1 : 0
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }
            }

            PlayerControls {
                id: controlButtons
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: root.spacing / 2
                    verticalCenter: parent.verticalCenter
                }
                opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }

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
                opacity: settings.introStep < Settings.IntroStepColors ? 0 : 1
                Behavior on opacity { UbuntuNumberAnimation { duration: UbuntuAnimation.SlowDuration } }

                UbuntuShape {
                    height: units.gu(2); width: parent.spacing; color: "red"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton {
                        anchors.fill: parent; anchors.margins: -10;
                        onClicked: {
                            if (settings.introStep == Settings.IntroStepColors) {
                                introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("red")
                            }
                            keys.red()
                        }
                    }
                }
                UbuntuShape {
                    height: units.gu(2); width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton {
                        anchors.fill: parent; anchors.margins: -10;
                        onClicked: {
                            if (settings.introStep == Settings.IntroStepColors) {
                                introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("green")
                            }
                            keys.green()
                        }
                    }
                }
                UbuntuShape {
                    height: units.gu(2); width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton {
                        anchors.fill: parent; anchors.margins: -10;
                        onClicked: {
                            if (settings.introStep == Settings.IntroStepColors) {
                                introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("yellow")
                            }
                            keys.yellow()
                        }
                    }
                }
                UbuntuShape {
                    height: units.gu(2); width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter;
                    AbstractButton {
                        anchors.fill: parent; anchors.margins: -10;
                        onClicked: {
                            if (settings.introStep == Settings.IntroStepColors) {
                                introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("blue")
                            }
                            keys.blue()
                        }
                    }
                }
            }
        }
    }

    BottomEdge {
        BottomEdgeButton {
            text: usePictureControls ? "Keypad" : "Pictures"
            source: usePictureControls ? "image://theme/keypad" : "image://theme/gallery-symbolic"
            visible: kodi.picturePlayerActive
            onClicked: {
                pictureControlsOverride = !pictureControlsOverride;
            }
        }
    }
}
