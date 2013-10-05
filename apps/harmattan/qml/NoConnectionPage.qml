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
import Xbmc 1.0

Page {
    id: mainPage
    tools: toolBarEntry

    XbmcDiscovery {
        id: discovery
    }

    Column {
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 100
            text: xbmc.connectionError
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
        Button{
            text: qsTr("Connect...")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                var component = Qt.createComponent("ConnectionSheet.qml")
                if (component.status === Component.Ready) {
                    component.createObject(mainPage).open()
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
        }
    }
}
