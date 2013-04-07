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

import Qt 4.7
import org.kde.plasma.components 0.1
import org.kde.plasma.core 0.1 as PlasmaCore

Row {
    id: root
    property int minimumHeight: dataColumn.height
    property variant activePlayer: xbmc.activePlayer
    property variant currentItem: activePlayer.currentItem

    Image {
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: height
        source: currentItem.thumbnail
        fillMode: Image.PreserveAspectFit
    }

    Column {
        id: dataColumn
        width: parent.width - x
        height: childrenRect.height

        Label {
            text: currentItem.title
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideRight
        }
        Label {
            text: "Year: " + currentItem.year
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideRight
        }
        Label {
            text: "Rating: " + currentItem.rating
            anchors { left: parent.left; right: parent.right }
            elide: Text.ElideRight
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
                value: activePlayer.percentage
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
                text: activePlayer.time
            }
            Label {
                anchors.right: parent.right
                text: activePlayer.currentItem.durationString
            }
        }
    }
}
