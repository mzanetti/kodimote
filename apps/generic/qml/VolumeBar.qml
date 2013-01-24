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

Item {
    id: bar
    anchors.left: parent.left
    anchors.right: parent.right
    height: 40
    property bool dragging: false

    function sliderPosToPercent(x) {
        return (slider.x) * 100 / (bar.width - slider.width)
    }
    function percentToSliderPos(pc) {
        return pc * (bar.width - slider.width) / 100;
    }

    Rectangle {
        id: container
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        height: 40
        width: parent.width
        radius: height / 2
        color: "#1d1d1d"
        border.color: "#333333"
        border.width: 2

        Rectangle {
            id: fill
            height: 40
            radius: height / 2
            color: "#8fb2c2"
            border.color: "#c7d9e2"
            border.width: 2
            anchors.fill: container

            anchors.rightMargin: container.width - slider.x - slider.width
        }

        Rectangle {
            id: slider
            color: "#c7d9e2"
            border.color: "#c7d9e2"
            border.width: 2
            height: 40
            width: 80
            radius: height / 2
            anchors.verticalCenter: parent.verticalCenter
            x: percentToSliderPos(xbmc.volume)


            MouseArea {
                anchors.fill: parent
                anchors.margins: -16 // Increase mouse area a lot outside the slider
                drag.target: parent; drag.axis: Drag.XAxis
                drag.minimumX: 2; drag.maximumX: container.width - slider.width - 2

                onReleased: xbmc.setVolume(sliderPosToPercent(slider.x))

            }
        }
    }
}
