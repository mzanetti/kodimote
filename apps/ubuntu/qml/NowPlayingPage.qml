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
    id: mainPage
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    property string orientation: width > height ? "landscape" : "portrait"

    property ActionList tools: ActionList {
        Action {
            text: "repeat"
        }
        Action {
            text: "shuffle"
        }
    }

    onPlayerChanged: {
        if(player === null) {
            if(mainPage.status === PageStatus.Active) {
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
        columns: mainPage.orientation == "portrait" ? 1 : 2
        spacing: appWindow.pageMargins
        anchors.margins: appWindow.pageMargins

        Item {
            id: imageItem
            height: mainPage.orientation == "portrait" ? parent.width : parent.height
            width: mainPage.orientation == "portrait" ? parent.width : height
            UbuntuShape {
                // iw : ih = uw : uh
                height: parent.height
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                radius: "medium"
                color: "black"
                //image:
                    Image {
                        anchors.fill: parent
                    source: !currentItem || currentItem.thumbnail.length === 0 ? "" : currentItem.thumbnail
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Item {
            id: textItem
            width: mainPage.orientation == "portrait" ? parent.width : parent.width - imageItem.width - parent.spacing
            height: mainPage.orientation == "portrait" ? parent.height - imageItem.height - parent.spacing : parent.height
            Label {
                id: currentTime
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: player ? player.time : "00:00"
            }

            Label {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: currentItem ? currentItem.durationString : "00:00"
            }

            Item {
                id: controlButtons
                anchors {left:parent.left; right: parent.right; bottom: progressBar.top }
                height: 50
                anchors.bottomMargin: 20
                Rectangle {
                    color: "green"
                    height: 50; width: 50
                    anchors.left: parent.left
                    MouseArea {
                        anchors.fill: parent
                        onClicked: player.skipPrevious();
                    }
                }
                Rectangle {
                    color: "blue"
                    height: 50; width: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: player.playPause();
                    }
                }
                Rectangle {
                    color: "red"
                    height: 50; width: 50
                    anchors.right: parent.right
                    MouseArea {
                        anchors.fill: parent
                        onClicked: player.skipNext();
                    }
                }
            }

            ProgressBar {
                id: progressBar
                anchors { left: parent.left; right: parent.right; bottom: currentTime.top }
                anchors.bottomMargin: 10
                width: 300
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
                    anchors.topMargin: -10
                    anchors.bottomMargin: -10
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
                id: albumRow
                anchors.bottom: controlButtons.top
                height: albumLabel.height
                anchors.left: parent.left
                anchors.bottomMargin: 10
                spacing: units.gu(.5)
                Label {
                    id: albumLabel
                    text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode : qsTr("Rating:") + " "))
                }
                property int starCount: !currentItem ? 0 : (currentItem.rating > 10 ? Math.floor(currentItem.rating / 20) : Math.floor(currentItem.rating / 2))
                Repeater {
                    model: parent.starCount
                    Rectangle {
                        height: units.gu(1)
                        width: units.gu(1)
                        radius: width / 2
                        anchors.verticalCenter: albumLabel.verticalCenter
                        visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                        color: "black"
                        //source: theme.inverted ? "image://theme/meegotouch-indicator-rating-inverted-background-star" : "image://theme/meegotouch-indicator-rating-star"
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Rectangle {
                        height: units.gu(1)
                        width: units.gu(1)
                        radius: width / 2
                        anchors.verticalCenter: albumLabel.verticalCenter
                        visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                        color: "#22000000"
                        //source: theme.inverted ? "image://theme/meegotouch-indicator-rating-background-star" : "image://theme/meegotouch-indicator-rating-background-star"
                    }
                }
            }

            Label {
                id: artistLabel
                anchors.bottomMargin: 6
                anchors.left: parent.left
                anchors.bottom: albumRow.top
                anchors.right: infoButton.left
                text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year))
            }
            Button {
                id: infoButton
                text: "i"
                width: height
                anchors.top: trackNumLabel.bottom
                anchors.right: parent.right

                onClicked: {
                    //listView.model.fetchItemDetails(listView.currentIndex)
                    pageStack.push(Qt.createComponent(Qt.resolvedUrl("NowPlayingDetails.qml")));
                }
            }

            Label {
                id: trackLabel
                anchors.left: parent.left
                //anchors.right: trackNumLabel.right
                width: parent.width - trackNumLabel.width
                anchors.bottom: artistLabel.top
                anchors.bottomMargin: 6
                text: currentItem ? currentItem.title : ""
                elide: Text.ElideRight
                font.bold: true
            }
            Label {
                id: trackNumLabel
                anchors.right: parent.right
                anchors.bottom: artistLabel.top
                anchors.bottomMargin: 6
                text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"
            }
        }
    }
}
