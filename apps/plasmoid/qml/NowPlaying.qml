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

import QtQuick 1.1
import org.kde.plasma.components 0.1
import org.kde.plasma.core 0.1 as PlasmaCore

Row {
    id: root
    property variant activePlayer: xbmc.activePlayer
    property variant playlist: activePlayer ? activePlayer.playlist() : null
    property variant currentItem: activePlayer ? activePlayer.currentItem : null


    Item {
        id: thumbnailItem
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: height
        Rectangle {
            anchors.fill: parent
            color: "black"
            visible: imageItem.status == Image.Ready
            Image {
                id: imageItem
                anchors.fill: parent
                source: currentItem ? currentItem.thumbnail : ""
                fillMode: Image.PreserveAspectFit
            }
        }
        PlasmaCore.IconItem {
            anchors.fill: parent
            source: xbmc.state == "audio" ? "audio-x-generic" : "video-x-generic"
            visible: imageItem.status != Image.Ready
        }

    }

    Column {
        id: dataColumn
        width: parent.width - thumbnailItem.width - root.spacing
        height: childrenRect.height
        anchors.bottom: parent.bottom

        Row {
            anchors { left: parent.left; right: parent.right }
            Label {
                text: currentItem ? currentItem.title : ""
                width: parent.width - trackNumLabel.width
                elide: Text.ElideRight
            }
            Label {
                id: trackNumLabel
                anchors.bottomMargin: 6
                text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"
            }
        }
        Label {
            text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year))
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideRight
        }
        Row {
            anchors { left: parent.left; right: parent.right }
            Label {
                text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode : qsTr("Rating:") + " "))
                elide: Text.ElideRight
            }

            property int starCount: !currentItem ? 0 : (currentItem.rating > 10 ? Math.floor(currentItem.rating / 20) : Math.floor(currentItem.rating / 2))
            Repeater {
                model: parent.starCount
                PlasmaCore.IconItem {
                    width: theme.iconSizes.small
                    height: width
                    source: "draw-star"
                    visible: currentItem ?(currentItem.type === "movie" || currentItem.type === "unknown") : false
                }
            }
            Repeater {
                model: 5 - parent.starCount
                PlasmaCore.IconItem {
                    width: theme.iconSizes.small
                    height: width
                    source: "draw-star"
                    visible: currentItem ? (currentItem.type === "movie" || currentItem.type === "unknown") : false
                    opacity: 0.3
                }
            }
        }

        PlayerControls {
            anchors { left: parent.left; right: parent.right }
            player: activePlayer
        }

        Slider {
            id: progress
            anchors { left: parent.left; right: parent.right }
            maximumValue: 100

            // Not implemented in plasma yet... Hopefully starts working one day
            valueIndicatorVisible: true
            valueIndicatorText:{
                if (!activePlayer) return ""
                var targetTime = 100 * activePlayer.durationInSecs / progress.value;
                targetTime = Math.min(targetTime, activePlayer.durationInSecs);
                targetTime = Math.max(targetTime, 0);

                // Translate to human readable time
                var hours = Math.floor(targetTime / 60 / 60);
                var minutes = Math.floor(targetTime / 60) % 60;
                if(minutes < 10) minutes = "0" + minutes;
                var seconds = Math.floor(targetTime) % 60;
                if(seconds < 10) seconds = "0" + seconds;

                // Write into the label
                if(activePlayer.durationInSecs < 60 * 60) {
                    return minutes + ":" + seconds;
                } else {
                    return hours + ":" + minutes + ":" + seconds;
                }

            }

            onPressedChanged: {
                if (!pressed) {
                    activePlayer.seek(value);
                }
            }

            Binding {
                target: progress
                property: "value"
                value: activePlayer ? activePlayer.percentage : 0
                when: !progress.pressed
            }
        }

        Item {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: childrenRect.height
            Label {
                anchors.left: parent.left
                text: activePlayer ? activePlayer.time : "0:00"
            }
            Label {
                anchors.right: parent.right
                text: activePlayer ? activePlayer.currentItem.durationString : "0:00"
            }
        }
    }
}
