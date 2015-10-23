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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3

Item {
    id: itemDetails

    signal playItem
    signal addToPlaylist
    signal download

    property var item

    Component.onCompleted: print("**************itemHeight: " + height)

    ListView {
        anchors {
            fill: parent
            leftMargin: units.gu(1)
            rightMargin: units.gu(1)
        }
        contentWidth: width
        contentHeight: labelColumn.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        z: 5


        model: itemDetails.item.channelBroadcasts
        delegate: Subtitled {
            text: title
            subText: Qt.formatDate(startTime) + "  " + Qt.formatTime(startTime) + " - " + Qt.formatTime(endTime)
            UbuntuShape {
                id: progressBar
                anchors {
                    left:parent.left
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: units.gu(.5)
                }
                color: "#22000000"

                height: units.dp(3)
                visible: model.isActive
                property int minimumValue: 0
                property int maximumValue: 100
                property int value: progressPercentage

                UbuntuShape {
                    anchors.fill: parent
                    anchors.rightMargin: parent.width - (parent.width * parent.value / 100)
                    color: "#1b62c8"
                }
            }
            UbuntuShape {
                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                height: units.gu(1)
                width: height
                color: "red"
                visible: hasTimer
            }
        }
    }
}
