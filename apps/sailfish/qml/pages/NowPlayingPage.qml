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
import "../components/"
import harbour.kodimote 1.0

Page {
    id: nowPlayingPage

    property QtObject player: kodi.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    onPlayerChanged: {
        if(player === null) {
            //pop immediately to prevent the empty page being shown during the pop animation
            pageStack.pop(undefined, PageStackAction.Immediate);
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("PlaylistPage.qml"));
        }
    }

    property bool timerActive: Qt.application.active && nowPlayingPage.status == PageStatus.Active

    onTimerActiveChanged: { player.timerActive = timerActive }

    SilicaFlickable {
        anchors.fill: parent

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
                onClicked: {
                    pageStack.replace("Keypad.qml")
                }
            }

            MenuItem {
                text: qsTr("Now playing")
                enabled: false
            }
        }

        PushUpMenu {
            MenuItem {
                text: player && player.state === "playing" ? qsTr("Pause") : qsTr("Play")
                onClicked: player.playPause()
            }

            MenuItem {
                text: qsTr("Next")
                onClicked: player.skipNext()
            }

            MenuItem {
                text: qsTr("Previous")
                onClicked: player.skipPrevious()
            }

            MenuItem {
                text: qsTr("Stop")
                onClicked: player.stop()
            }
        }

        contentHeight: column.childrenRect.height

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Theme.paddingLarge

            // create space to display thumbnail below navigation bullets
            PageHeader {

            }

            Thumbnail {
                artworkSource: currentItem ? currentItem.thumbnail : ""
                width: parent.width
                height: artworkSize && artworkSize.width > artworkSize.height ? artworkSize.height / (artworkSize.width / width) : 400
                fillMode: Image.PreserveAspectFit
                smooth: true
                defaultText: currentItem ? currentItem.title : ""

                MouseArea {
                    anchors.fill: parent
                    onClicked: drawer.open = !drawer.open
                }
            }

            // create some space between thumbnail and title
            Item{
                width: parent.width
                height: Theme.paddingLarge

                MouseArea {
                    anchors.fill: parent
                    onClicked: drawer.open = !drawer.open
                }
            }

            Item {
                height: titleLabel.height
                anchors.left: parent.left
                anchors.right: parent.right
                Label {
                    id: titleLabel
                    anchors {
                        left: parent.left
                        right: playlistItemLabel.left
                        rightMargin: Theme.paddingMedium
                    }
                    font {
                        bold: true
                        family: Theme.fontFamilyHeading
                    }
                    elide: Text.ElideRight
                    text: currentItem ? currentItem.title : ""

                    MouseArea {
                        anchors.fill: parent
                        onClicked: drawer.open = !drawer.open
                    }
                }

                Label {
                    id: playlistItemLabel
                    anchors.right: parent.right
                    elide: Text.ElideRight
                    text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"

                    Behavior on opacity {
                        NumberAnimation {
                            property: "opacity"
                            duration: 300
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.navigateForward()
                    }
                }
            }

            Label {
                text: currentItem.subtitle
                width: parent.width
                elide: Text.ElideRight
                color: Theme.highlightColor
                visible: text.length > 0
                font {
                    family: Theme.fontFamilyHeading
                    bold: true
                }
            }

            Label {
                text: currentItem.album
                width: parent.width
                elide: Text.ElideRight
                visible: text.length > 0
                font.bold: true
            }

            ItemDetailRow {
                visible: currentItem.season > -1
                title: qsTr("Season:")
                text: currentItem.season
            }

            ItemDetailRow {
                visible: currentItem.episode > -1
                title: qsTr("Episode:")
                text: currentItem.episode
            }

            Drawer {
                id: drawer
                backgroundSize: itemDetails.height
                property real backgroundHeight: itemDetails.height * drawer._progress

                background: NowPlayingDetails {
                    id: itemDetails

                    width: parent.width
                }

                height: playerColumn.height < backgroundHeight ? backgroundHeight : playerColumn.height
                width: parent.width

                Column {
                    id: playerColumn
                    width: parent.width

                    ProgressBar {
                        id: progressBar
                        width: parent.width
                        leftMargin: 0
                        rightMargin: 0

                        minimumValue: 0
                        maximumValue: 100
                        value: player ? player.percentage : 0

                        Label {
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom
                            color: Theme.highlightColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            text: player ? player.timeString : "00:00"
                        }

                        Label {
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            color: Theme.highlightColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            text: currentItem ? currentItem.durationString : "00:00"
                        }

                        Rectangle {
                            color: Theme.primaryColor
                            rotation: 45
                            width: 10
                            height: 10
                            anchors.horizontalCenter: progressBarLabel.horizontalCenter
                            anchors.verticalCenter: progressBarLabel.bottom
                            visible: progressBarLabel.visible
                        }

                        Rectangle {
                            id: progressBarLabel
                            color: Theme.primaryColor
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: (Theme.paddingLarge * 2) + Theme.fontSizeSmall
                            height: 40
                            width: progressBarLabelText.width + 20
                            radius: 5
                            visible: progressBarMouseArea.pressed

                            Label {
                                id: progressBarLabelText
                                anchors.centerIn: parent
                                color: Theme.secondaryHighlightColor
                            }
                        }

                        MouseArea {
                            id: progressBarMouseArea
                            height: Theme.paddingLarge
                            width: parent.width
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: Theme.fontSizeSmall
                            preventStealing: true

                            onMouseXChanged: {
                                // Center label on mouseX
                                progressBarLabel.x = mouseX - progressBarLabel.width / 2;

                                // Calculate time under mouseX
                                var progressedWidth = mouseX - progressBar.x;
                                var targetTime = progressedWidth * currentItem.durationInSecs / progressBar.width;
                                targetTime = Math.min(targetTime, currentItem.durationInSecs);
                                targetTime = Math.max(targetTime, 0);

                                // Translate to human readable time
                                var hours = Math.floor(targetTime / 60 / 60);
                                var minutes = Math.floor(targetTime / 60) % 60;
                                if(minutes < 10) minutes = "0" + minutes;
                                var seconds = Math.floor(targetTime) % 60;
                                if(seconds < 10) seconds = "0" + seconds;

                                // Write into the label
                                if(currentItem.durationInSecs < 60 * 60) {
                                    progressBarLabelText.text = minutes + ":" + seconds;
                                } else {
                                    progressBarLabelText.text = hours + ":" + minutes + ":" + seconds;
                                }
                            }

                            onReleased: {
                                player.seek(mouseX * 100 / width)
                            }
                        }
                    }
                }

                states: [
                    State {
                        when: drawer.opened
                        PropertyChanges { target: playlistItemLabel; opacity: 0 }
                    }
                ]
            }
        }
    }
}
