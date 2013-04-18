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

Item {
    id: root
    width: units.gu(2)
    height: units.gu(2)

    property string source
    property bool enabled: true
    property bool selected: false
    property string text: ""
    property alias font: label.font

    signal clicked()

    Row {
        anchors.fill: parent

        Icon {
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            width: height
            name: root.source
        }
        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            text: root.text
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            rumbleEffect.start(2);
            root.clicked();
        }
    }
}
