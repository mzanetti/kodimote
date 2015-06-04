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
import Ubuntu.Components 1.2

AbstractButton {
    id: root
    height: text.length > 0 ? units.gu(6) : units.gu(4)
    implicitWidth: Math.max(image.width, label.width)

    property string source
    property bool enabled: true
    property bool selected: false
    property alias text: label.text

    XIcon {
        id: image
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top }
        width: units.gu(4)
        height: width
        name: root.source
        color: "black"
    }

    Label {
        id: label
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom }
        fontSize: "x-small"
        color: "black"
    }
}
