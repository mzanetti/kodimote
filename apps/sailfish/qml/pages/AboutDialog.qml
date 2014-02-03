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
import Sailfish.Silica 1.0

Dialog {

    Column {
        anchors.fill: parent
        anchors.topMargin: Theme.itemSizeLarge
        anchors.leftMargin: Theme.paddingSmall
        anchors.rightMargin: Theme.paddingSmall
        spacing: Theme.paddingSmall

        Label {
            font {
                pixelSize: Theme.fontSizeLarge
                family: Theme.fontFamilyHeading
            }
            color: Theme.highlightColor
            text: "Xbmcremote 1.5"
        }

        Label {
            font.pixelSize: Theme.fontSizeSmall
            text: "Michael Zanetti<br>michael_zanetti@gmx.net<br>http://notyetthere.org/maemomeego/xbmcremote" +
                  "<br>Sailfish port by:<br>Robert Meijers<br>robert.meijers@gmail.com";
        }

        SilicaFlickable {
            width: parent.width
            height: parent.height - y - (donateButton.height * 2)
            contentHeight: gplLabel.implicitHeight
            clip: true

            Label {
                id: gplLabel
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall * 0.75
                text: "This program is free software: you can redistribute it and/or modify " +
                      "it under the terms of the GNU General Public License as published by " +
                      "the Free Software Foundation, either version 3 of the License, or " +
                      "(at your option) any later version.\n\n" +

                      "This program is distributed in the hope that it will be useful, " +
                      "but WITHOUT ANY WARRANTY; without even the implied warranty of " +
                      "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the " +
                      "GNU General Public License for more details.\n\n" +

                      "You should have received a copy of the GNU General Public License " +
                      "along with this program.  If not, see http://www.gnu.org/licenses/."
            }
        }

        Button {
            id: donateButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Donate")
            onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2")
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Flattr")
            onClicked: Qt.openUrlExternally("http://flattr.com/thing/412274/Xbmcremote")
        }
    }
}
