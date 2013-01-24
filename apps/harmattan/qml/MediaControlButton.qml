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
import com.nokia.meego 1.0

Item {
    id: root
    width: 80
    height: 80

    property string platformIconId
    property string iconSource
    property string source
    property bool enabled: true
    property bool selected: false
    property string text: ""
    property bool showBorder: false
    property alias font: label.font

    signal clicked()

    Rectangle {
        id: borderRect
        anchors.fill: parent
        border.color: "white"
        border.width: 5
        radius: width / 2
        color: "black"
        visible: root.showBorder
    }
    Label {
        id: label
        anchors.centerIn: parent
        text: root.text
    }

    Image {
        anchors.centerIn: parent
        height: 40
        width: 40
        smooth: true

        function handleIconSource(iconId, iconSource, source, isEnabled, isSelected) {
            if (source.length > 0) {
                return source;
            }

            if (iconSource.length > 0) {
                return "image://theme/" + iconSource;
            }

            if (iconId.length > 0) {
                if (!isEnabled)
                    iconId = iconId + "-dimmed";

                if (theme.inverted)
                    iconId = iconId + "-white";

                if (isSelected)
                    iconId = iconId + "-selected";

                return "image://theme/icon-m-" + iconId;
            }

            return ""
        }

        source: handleIconSource(root.platformIconId, root.iconSource, root.source, root.enabled, root.selected)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked();
    }
}
