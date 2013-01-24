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

Image {
    id: button
    property string icon
    signal pressed()
    signal pressAndHold()
    signal released()
    source: "icons/" + button.icon + ".png"

    property int mouseAreaLeftMargin: 0
    property int mouseAreaRightMargin: 0
    property int mouseAreaTopMargin: 0
    property int mouseAreaBottomMargin: 0

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        anchors.leftMargin: button.mouseAreaLeftMargin
        anchors.rightMargin: button.mouseAreaRightMargin
        anchors.topMargin: button.mouseAreaTopMargin
        anchors.bottomMargin: button.mouseAreaBottomMargin
        onPressed: rumbleEffect.start();
        onClicked:button.pressed();
        onPressAndHold: button.pressAndHold();
        onReleased: button.released();
    }
    states: [
        State {
            when: mouseArea.pressed;
            PropertyChanges { target: button; source: "icons/" + button.icon + "_pressed.png" }
        }
    ]

//    HapticsEffect {
//        id: rumbleEffect
//        attackIntensity: 0.0
//        attackTime: 250
//        intensity: 1.0
//        duration: 100
//        fadeTime: 250
//        fadeIntensity: 0.0
//    }
}
