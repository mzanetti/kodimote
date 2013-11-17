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
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1
import Xbmc 1.0
import "components/ToolbarButtons" as ToolbarButtons

Page {
    id: root
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    property string orientation: width > height ? "landscape" : "portrait"

    property int spacing: units.gu(1)

    onCurrentItemChanged: print("************", currentItem.thumbnail)

    tools: ToolbarItems {
        ToolbarButtons.SystemMenu {}
        ToolbarButtons.Spacer {}
        ToolbarButton {
            iconSource: "image://theme/media-playlist-repeat"
            text: "repeat"
            visible: xbmc.activePlayer && xbmc.activePlayer.type == Player.PlayerTypeAudio
        }
        ToolbarButton {
            text: "shuffle"
            iconSource: "image://theme/media-playlist-shuffle"
            visible: xbmc.activePlayer && xbmc.activePlayer.type == Player.PlayerTypeAudio
        }
        ToolbarButton {
            iconSource: "image://theme/messages"
            text: qsTr("Subtitle")
            visible: xbmc.activePlayer && xbmc.activePlayer.type == Player.PlayerTypeVideo
            enabled: !(xbmc.state === "video" && player.subtitles.length === 0)
            onTriggered: {
                var popup = PopupUtils.open(toolbarMenuComponent, audioTrackButton, {model: player.subtitles, currentIndex: player.currentSubtitle})
                popup.accepted.connect(function(selectedIndex) {player.currentSubtitle = selectedIndex})
            }
        }
        ToolbarButton {
            id: audioTrackButton
            text: qsTr("Audio track")
            iconSource: "image://theme/speaker"
            visible: xbmc.activePlayer && xbmc.activePlayer.type == Player.PlayerTypeVideo
            //enabled: player.audiostreams.size > 0
            onTriggered: {
                var popup = PopupUtils.open(toolbarMenuComponent, audioTrackButton, {model: player.audiostreams, currentIndex: player.currentAudiostream})
                popup.accepted.connect(function(selectedIndex) {player.currentAudiostream = selectedIndex})
            }
        }
    }

    onPlayerChanged: {
        if(player === null) {
            if(root.status === PageStatus.Active) {
                pageStack.pop();
            }
        }
    }

    property bool timerActive: false

    onTimerActiveChanged: {
        print("timer is now", timerActive)
        player.timerActive = timerActive
    }


   Connections {
       target: xbmc
       onStateChanged: {
           print("!*!*! state:", xbmc.state);
       }
   }

    Component.onCompleted: {
        console.log("player is " + player)
        console.log("playlist is " + playlist)
    }

    Label {
        anchors.centerIn: parent
        width: parent.width - units.gu(4)
        text: qsTr("No active playback. Please select some content from the media tab.")
        wrapMode: Text.WordWrap
        fontSize: "large"
        opacity: xbmc.activePlayer == null ? 1 : 0
        visible: opacity > 0
        horizontalAlignment: Text.AlignHCenter

        Behavior on opacity {
            UbuntuNumberAnimation {}
        }
    }

    Item {
        anchors.fill: parent
        opacity: xbmc.activePlayer !== null ? 1 : 0
        visible: opacity > 0
        Behavior on opacity {
            UbuntuNumberAnimation {}
        }

        ListView {
            id: playlistView
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            height: showPlaylist ? parent.height - units.gu(8) : 0
            property bool showPlaylist: false
            clip: true
            Behavior on height {
                UbuntuNumberAnimation {}
            }

            Rectangle {
                anchors.fill: parent
                color: Qt.rgba(0, 0, 0, 0.3)
            }

            model: playlist
            delegate: SingleValue {
                id: playlistDelegate
                text: title
                value: duration
                selected: index === ListView.view.model.currentTrackNumber - 1
                onClicked: {
                    player.playItem(index);
                }
                onPressAndHold: {
                    PopupUtils.open(playlistPopoverComponent, playlistDelegate, {selectedIndex: index})
                }
            }
        }

        MouseArea {
            anchors {
                left: parent.left
                right: parent.right
                top: playlistView.bottom
                bottom: parent.bottom
            }
            enabled: playlistView.showPlaylist
            onClicked: playlistView.showPlaylist = false

            z: 2
        }

        Grid {
            id: mainGrid
            anchors {
                left: parent.left
                right: parent.right
                top: playlistView.bottom
                leftMargin: appWindow.pageMargins
                rightMargin: appWindow.pageMargins
                topMargin: appWindow.pageMargins
            }
            height: parent.height - appWindow.pageMargins * 2
//            anchors.fill: parent

            columns: root.orientation == "portrait" ? 1 : 2
            spacing: appWindow.pageMargins
            opacity: playlistView.showPlaylist ? .6 : 1
            Behavior on opacity { UbuntuNumberAnimation {} }

            Item {
                id: imageItem
                height: root.orientation == "portrait" ? Math.min(parent.width, parent.height - textItem.height - parent.spacing) : parent.height
                width: root.orientation == "portrait" ? parent.width : height

                LazyImage {
                    id: imageShape
                    anchors.fill: parent
                    opacity: 1
                    source: !currentItem || currentItem.thumbnail.length === 0 ? "" : currentItem.thumbnail
                    initialWidth: parent.width
                    initialHeight: parent.height
                    scaleTo: "fit"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: playlistView.showPlaylist = true
                    }
                }
            }

            Column {
                id: textItem
                width: root.orientation == "portrait" ? parent.width : parent.width - imageItem.width - parent.spacing
                spacing: root.spacing
                Row {
                    width: parent.width
                    spacing: root.spacing
                    Label {
                        id: trackLabel
                        width: parent.width - trackNumLabel.width - parent.spacing
                        text: currentItem ? currentItem.title : ""
                        elide: Text.ElideRight
                        font.bold: true
                    }
                    Label {
                        id: trackNumLabel
                        text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"
                    }
                }
                Label {
                    id: artistLabel
                    width: parent.width
                    elide: Text.ElideRight
                    text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year))
                }
                Row {
                    id: albumRow
                    height: albumLabel.height
                    width: parent.width
                    spacing: units.gu(.5)
                    Label {
                        id: albumLabel
                        width: Math.min(implicitWidth, parent.width)
                        elide: Text.ElideRight
                        text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode : qsTr("Rating:") + " "))
                    }
                    property int starCount: !currentItem ? 0 : (currentItem.rating > 10 ? Math.floor(currentItem.rating / 20) : Math.floor(currentItem.rating / 2))

                    RatingStars {
                        count: 5
                        rated: parent.starCount
                        height: albumLabel.height
                        visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                    }
                }

                PlayerControls {
                    id: controlButtons
                    player: root.player
                    width: parent.width
                }


                UbuntuShape {
                    id: progressBar
                    width: parent.width
                    height: units.gu(1)
                    property int minimumValue: 0
                    property int maximumValue: 100
                    property int value: player ? player.percentage : 0

                    UbuntuShape {
                        anchors.fill: parent
                        anchors.rightMargin: parent.width - (parent.width * parent.value / 100)
                        color: "#1b62c8"
                    }

                    Rectangle {
                        color: "black"
                        rotation: 45
                        width: 10
                        height: 10
                        anchors.horizontalCenter: progressBarLabel.horizontalCenter
                        anchors.verticalCenter: progressBarLabel.bottom
                        visible: progressBarLabel.visible
                    }

                    UbuntuShape {
                        id: progressBarLabel
                        color: "black"
                        anchors.bottom: parent.top
                        anchors.bottomMargin: 40
                        height: 40
                        width: progressBarLabelText.width + 20
                        visible: progressBarMouseArea.pressed

                        Label {
                            id: progressBarLabelText
                            anchors.centerIn: parent
                            color: "white"
                        }
                    }

                    MouseArea {
                        id: progressBarMouseArea
                        anchors.fill: progressBar
                        anchors.topMargin: -root.spacing
                        anchors.bottomMargin: -root.spacing
                        preventStealing: true
                        property var targetTime
                        drag.target: anchorItem

                        onPositionChanged: {
                            // Center label on mouseX
                            progressBarLabel.x = mouseX - progressBarLabel.width / 2;

                            // Calculate time under mouseX
                            var progressedWidth = mouseX - progressBar.x;
                            var targetTime = progressedWidth * currentItem.durationInSecs / progressBar.width;
                            targetTime = Math.min(targetTime, currentItem.durationInSecs);
                            targetTime = Math.max(targetTime, 0);
                            print("currentItem", currentItem.durationInSecs)

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
                            mouse.accepted = true
                            print("mouse accepted")
                        }
                        onReleased: {
                            //player.seek(mouseX * 100 / width)
                        }
                    }
                }

                Row {
                    width: parent.width
                    spacing: root.spacing
                    Label {
                        id: currentTime
                        text: player ? player.time : "00:00"
                        width: (parent.width - parent.spacing) / 2
                    }

                    Label {
                        text: currentItem ? currentItem.durationString : "00:00"
                        horizontalAlignment: Text.AlignRight
                        width: (parent.width - parent.spacing) / 2
                    }
                }
            }
        }
    }


    Component {
        id: toolbarMenuComponent
        Popover {
            id: toolbarMenu
            property alias model: listView.model
            property alias currentIndex: listView.currentIndex

            signal accepted(int selectedIndex)

            ListView {
                id: listView
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
                height: contentHeight
                model: toolbarMenuModel
                delegate: Standard {
                    text: modelData
                    selected: listView.currentIndex == index
                    onClicked: {
                        toolbarMenu.accepted(index)
                        PopupUtils.close(toolbarMenu)
                    }
                }
            }
        }
    }

    Component {
        id: playlistPopoverComponent
        Popover {
            id: playlistPopover
            property int selectedIndex
            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
                height: childrenRect.height

                Standard {
                    text: qsTr("Play")
                    onClicked: {
                        player.playItem(selectedIndex)
                        PopupUtils.close(playlistPopover)
                    }
                }
                Standard {
                    text: qsTr("Remove from playlist")
                    onClicked: {
                        playlist.removeItem(selectedIndex)
                        PopupUtils.close(playlistPopover)
                    }
                }
                Standard {
                    text: qsTr("Clear playlist")
                    onClicked: {
                        playlist.clear()
                        PopupUtils.close(playlistPopover)
                    }
                }
            }
        }
    }
}
