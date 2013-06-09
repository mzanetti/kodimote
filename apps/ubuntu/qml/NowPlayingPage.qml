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

Page {
    id: root
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    property string orientation: width > height ? "landscape" : "portrait"

    property int spacing: units.gu(1)

    tools: ToolbarActions {
        Action {
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/reload.svg"
            text: "repeat"
        }
        Action {
            text: "shuffle"
            iconSource: "/usr/share/icons/ubuntu-mobile/actions/scalable/help.svg"
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

    Grid {
        id: mainGrid
        anchors.fill: parent
        columns: root.orientation == "portrait" ? 1 : 2
        spacing: appWindow.pageMargins
        anchors.margins: appWindow.pageMargins

        Item {
            id: imageItem
            height: root.orientation == "portrait" ? Math.min(parent.width, parent.height - textItem.height - parent.spacing) : parent.height
            width: root.orientation == "portrait" ? parent.width : height
            UbuntuShape {
                // iw : ih = uw : uh
                height: parent.height
                width: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                radius: "medium"
                color: "black"
                image:
                    Image {
                        anchors.fill: parent
                    source: !currentItem || currentItem.thumbnail.length === 0 ? "" : currentItem.thumbnail
                    fillMode: Image.PreserveAspectFit
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
                text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year))
            }
            Row {
                id: albumRow
                height: albumLabel.height
                spacing: units.gu(.5)
                Label {
                    id: albumLabel
                    text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode : qsTr("Rating:") + " "))
                }
                property int starCount: !currentItem ? 0 : (currentItem.rating > 10 ? Math.floor(currentItem.rating / 20) : Math.floor(currentItem.rating / 2))
                Repeater {
                    model: parent.starCount
                    Icon {
                        height: units.gu(2)
                        width: units.gu(2)
                        name: "/usr/share/icons/ubuntu-mobile/actions/scalable/favorite-selected.svg"
                        anchors.verticalCenter: albumLabel.verticalCenter
                        visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                        //source: theme.inverted ? "image://theme/meegotouch-indicator-rating-inverted-background-star" : "image://theme/meegotouch-indicator-rating-star"
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Icon {
                        height: units.gu(2)
                        width: units.gu(2)
                        name: "/usr/share/icons/ubuntu-mobile/actions/scalable/favorite-unselected.svg"
                        anchors.verticalCenter: albumLabel.verticalCenter
                        visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                        //source: theme.inverted ? "image://theme/meegotouch-indicator-rating-background-star" : "image://theme/meegotouch-indicator-rating-background-star"
                    }
                }
            }
            PlayerControls {
                id: controlButtons
                player: root.player
                width: parent.width
            }


            ProgressBar {
                id: progressBar
                width: parent.width
                //height: gu.height(1)
                minimumValue: 0
                maximumValue: 100
                value: player ? player.percentage : 0

                Rectangle {
                    color: "black"
                    rotation: 45
                    width: 10
                    height: 10
                    anchors.horizontalCenter: progressBarLabel.horizontalCenter
                    anchors.verticalCenter: progressBarLabel.bottom
                    visible: progressBarLabel.visible
                }

                Rectangle {
                    id: progressBarLabel
                    color: "black"
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 40
                    height: 40
                    width: progressBarLabelText.width + 20
                    radius: 5
                    visible: progressBarMouseArea.pressed

                    Label {
                        id: progressBarLabelText
                        anchors.centerIn: parent
                        color: "black"
                    }
                }

                MouseArea {
                    id: progressBarMouseArea
                    anchors.fill: progressBar
                    anchors.topMargin: -root.spacing
                    anchors.bottomMargin: -root.spacing
                    preventStealing: true

                    onPositionChanged: {
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
                        mouse.accepted = true
                        print("mouse accepted")
                    }
                    onReleased: {
                        player.seek(mouseX * 100 / width)
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
