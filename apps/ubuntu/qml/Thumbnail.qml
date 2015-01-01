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

import QtQuick 2.0
import Ubuntu.Components 0.1

Loader {
    width: 100
    height: 100

    sourceComponent: artworkSource.length > 0 ? artworkItem : defaultTextItem

    property string artworkSource
    property string defaultText
    property string testColor: "green"

//    Component {
//        id: testItem
//        Rectangle {
//            color: testColor
//            anchors.fill: parent
//        }
//    }

    Component {
        id: artworkItem
        Image {
            source: artworkSource
            anchors.fill: parent
        }
    }

    Component {
        id: defaultTextItem
        Text {
            text: "<b>" + defaultText + "</b> " + defaultText + " " + defaultText
            wrapMode: Text.WrapAnywhere
            anchors.fill: parent
            font.pointSize: height / 6
            clip: true
            property int colorNr: Math.floor(Math.random() * 5)
            color:  colorNr == 0  ? "lightblue" : colorNr == 1 ? "red" : colorNr == 2 ? "lightgreen" : colorNr == 3 ? "yellow" : "white"
        }
    }

}
