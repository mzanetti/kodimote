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

    onStatusChanged: {
        if (status === PageStatus.Active) {
            dockedControls.hideTemporary = settings.introStep < Settings.IntroStepDone
        } else {
            dockedControls.hideTemporary = false
        }
    }

    Connections {
        target: settings
        onIntroStepChanged: {
            dockedControls.hideTemporary = settings.introStep < Settings.IntroStepDone
        }
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
                text: qsTr("Now Playing")
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

                Label {
                    id: introLabel1
                    anchors {fill: parent; leftMargin: Theme.paddingSmall; rightMargin: Theme.paddingSmall; }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    opacity: settings.introStep < Settings.IntroStepDone ? 1 : 0
                    Behavior on opacity { NumberAnimation { duration: 500 } }
                    color: "white"
                    text: {
                        switch (settings.introStep) {
                        case Settings.IntroStepLeftRight:
                            return qsTr("To move left or right, swipe horizontally anywhere on the pad.");
                        case Settings.IntroStepUpDown:
                            return qsTr("To move up or down, swipe vertically.");
                        case Settings.IntroStepScroll:
                            switch (gesturePad.scrollCounter) {
                            case 0:
                                return qsTr("To scroll through lists keep holding after swiping.");
                            case 1:
                                return qsTr("You've scrolled 1 time, keep holding to scroll another 9 times.");
                            default:
                                return qsTr("You've scrolled %1 times, keep holding to scroll another %2 times.").arg(gesturePad.scrollCounter).arg(10 - gesturePad.scrollCounter);
                            }
                        case Settings.IntroStepClick:
                            return qsTr("To select an item, tap anywhere on the pad.");
                        case Settings.IntroStepColors:
                            return qsTr("Pro tip: The color buttons at the bottom simulate an infrared remote.")
                        case Settings.IntroStepExit:
                            return qsTr("Tap the pad to finish the tutorial.")
                        }
                        return ""
                    }
                }

                Row {
                    opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }
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
                    opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }
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
                    opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }
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
                    opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }
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
                    opacity: settings.introStep < Settings.IntroStepDone ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }
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
                    opacity: settings.introStep < Settings.IntroStepColors ? 0 : 1
                    Behavior on opacity { NumberAnimation { duration: 500 } }

                    Rectangle {
                        height: 20; width: parent.spacing; color: "red"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea {
                            anchors.fill: parent; anchors.margins: -10;
                            onClicked: {
                                if (settings.hapticsEnabled) {
                                    rumbleEffect.start(2);
                                }

                                if (settings.introStep < Settings.IntroStepDone) {
                                    introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("red")
                                    settings.introStep = Settings.IntroStepExit;
                                }

                                keys.red();
                            }
                        }
                    }
                    Rectangle {
                        height: 20; width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea {
                            anchors.fill: parent; anchors.margins: -10;
                            onClicked: {
                                if (settings.hapticsEnabled) {
                                    rumbleEffect.start(2);
                                }

                                if (settings.introStep < Settings.IntroStepDone) {
                                    introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("green")
                                    settings.introStep = Settings.IntroStepExit;
                                }

                                keys.green()
                            }
                        }
                    }
                    Rectangle {
                        height: 20; width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea {
                            anchors.fill: parent; anchors.margins: -10;
                            onClicked: {
                                if (settings.hapticsEnabled) {
                                    rumbleEffect.start(2);
                                }

                                if (settings.introStep < Settings.IntroStepDone) {
                                    introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("yellow")
                                    settings.introStep = Settings.IntroStepExit;
                                }

                                keys.yellow()
                            }
                        }
                    }
                    Rectangle {
                        height: 20; width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                        MouseArea {
                            anchors.fill: parent; anchors.margins: -10;
                            onClicked: {
                                if (settings.hapticsEnabled) {
                                    rumbleEffect.start(2);
                                }

                                if (settings.introStep < Settings.IntroStepDone) {
                                    introLabel2.text = qsTr("Remote name: %1<br>Button name: %2").arg("kodimote").arg("blue")
                                    settings.introStep = Settings.IntroStepExit;
                                }

                                keys.blue()
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                height: Theme.itemSizeMedium
                visible: settings.introStep < Settings.IntroStepDone
                Label {
                    id: introLabel2
                    anchors {fill: parent; leftMargin: Theme.paddingSmall; rightMargin: Theme.paddingSmall; }
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
                    opacity: settings.introStep < Settings.IntroStepDone ? 1 : 0
                    Behavior on opacity { NumberAnimation { duration: 500 } }
                }
            }
        }
    }
}
