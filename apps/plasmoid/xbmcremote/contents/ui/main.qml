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

Item {
    id: root

    property int minimumWidth: popupDialog.minimumWidth
    property int minimumHeight: popupDialog.minimumHeight

    property Component compactRepresentation: Rectangle {
        color: "red"
    }


    PlasmaCore.DataSource {
        id: dataSource
        dataEngine: "xbmcremote"
        connectedSources: ['Hosts', 'Xbmc']
        //interval: 5000


        onNewData: {
            if (sourceName == 'Hosts') {
                print("going to connect to " +  data[0])
                var description = serviceForSource('Hosts').operationDescription("Connect");
                description.id = 0;
                //serviceForSource('Hosts').startOperationCall(description)
                //activeSource = data['Hosts'][0]
                //connectedSources = ['Hosts', activeSource]
            }
        }

    }

    PlasmaCore.Theme {
        id: theme
    }

    PopupDialog {
        id: popupDialog
        anchors.fill: parent
    }
}
