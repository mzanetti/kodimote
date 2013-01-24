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

Dialog {
    id: downloadDialog
    property string itemName

    title: Item {
        height: 50
        width: parent.width
        Text {
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 32
            text: qsTr("Start download?")
        }

    }

    content:Item {
        id: name
        height: 100
        width: parent.width
        Label {
            id: text
            font.pixelSize: 22
            anchors.centerIn: parent
            color: "white"
            text: qsTr("Start downloading %1?").arg(downloadDialog.itemName)
        }
    }

    buttons {
        ButtonRow {
            width: parent.width
            Button {
                id: closeButton
                text: qsTr("Yes")
                onClicked: downloadDialog.accept();
            }
            Button {
                id: donateButton
                text: qsTr("No")
                onClicked: downloadDialog.reject();
            }
        }
    }
}
