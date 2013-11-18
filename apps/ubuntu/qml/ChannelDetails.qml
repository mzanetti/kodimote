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
import Ubuntu.Components.ListItems 0.1

Item {
    id: itemDetails

    signal playItem
    signal addToPlaylist
    signal download

    property var item

    Component.onCompleted: print("**************itemHeight: " + height)

    Flickable {
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

        Column {
            id: labelColumn
            spacing: units.gu(1)
            height: childrenRect.height + units.gu(2)
            anchors {
                top: parent.top
                topMargin: units.gu(1)
                left: parent.left
                right: parent.right
            }

//            Row {
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.margins: units.gu(2)
//                height: units.gu(6)
//                spacing: units.gu(1)

//                LazyImage {
//                    height: parent.height
//                    width: height
//                    scaleTo: "fit"
//                    source: thumbnail
//                    initialWidth: parent.width
//                    initialHeight: width
//                }
//                Label {
//                    width: parent.width - x
//                    anchors.verticalCenter: parent.verticalCenter
//                    text: title
//                    fontSize: "large"
//                }
//            }

            Repeater {
                model: itemDetails.item.channelBroadcasts
                delegate: Subtitled {
                    text: title
                    subText: Qt.formatTime(startTime) + " - " + Qt.formatTime(endTime)
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
    }
}
