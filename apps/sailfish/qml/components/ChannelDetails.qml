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

Item {
    id: itemDetails

    signal playItem
    signal addToPlaylist
    signal download

    property QtObject item

    ListView {
        id: epgList
        anchors.fill: parent
        model: itemDetails.item.channelBroadcasts

        delegate: ListItem {
            width: epgList.width
            contentHeight: model.isActive ? column.height + 20 : column.height

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }
                id: column
                Label {
                    width: parent.width
                    color: Theme.primaryColor
                    elide: Text.ElideRight
                    text: title
                }

                Label {
                    width: parent.width
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    elide: Text.ElideRight
                    text: Qt.formatDate(startTime) + "  " + Qt.formatTime(startTime) + " - " + Qt.formatTime(endTime)
                }
            }

            Rectangle {
                anchors { right: parent.right; verticalCenter: parent.verticalCenter; rightMargin: Theme.paddingSmall }
                color: "red"
                height: 15
                width: height
                radius: height * 0.5
                visible: hasTimer
            }


            ProgressBar {
                width: parent.width
                minimumValue: 0
                maximumValue: 100
                value: progressPercentage
                visible: model.isActive

                leftMargin: Theme.paddingLarge
                rightMargin: Theme.paddingLarge
                height: 20
                anchors { verticalCenter: parent.bottom; verticalCenterOffset: 20 }
            }
        }

        VerticalScrollDecorator { }
    }
}
