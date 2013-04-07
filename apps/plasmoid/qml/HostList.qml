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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

import Xbmc 1.0

Item {
    id: root
    property bool discovering: false

    signal hostSelected(string hostname)

    XbmcDiscovery {
        id: discovery
        continuousDiscovery: root.discovering
    }

    ListView {
        id: hostListView
        anchors.fill: parent
        model: xbmc.hostModel()

        delegate: PlasmaComponents.ListItem {
            height: 50
            width: parent.width
            PlasmaComponents.Label {
                id: hostLabel
                anchors.fill: parent
                text: hostname
                width: parent.width
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    xbmc.hostModel().connectToHost(index)
                    root.hostSelected(hostname)
                }
            }
        }
    }
}
