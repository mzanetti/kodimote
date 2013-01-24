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

import Qt 4.7
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts


QGraphicsWidget {
    id: page;
    preferredSize: "200x200"
    minimumSize: "200x20"
    property string activeSource: ''
    
    Item {
        id:main

         PlasmaCore.DataSource {
            id: dataSource
            dataEngine: "xbmcremote"
            connectedSources: ['Hosts']
            interval: 5000

	    onNewData: {
	      print("got new data", data[0])
	      print("data size", data.lenght)
            }

            onDataChanged: {
	      print("got new data", data['Hosts'].volume)
            }
        }
        
        PlasmaCore.Theme {
                id: theme
        }
        
    }

    Component.onCompleted:
    {
        dataSource.serviceForSource(activeSource).associateWidget(stop, "stop");
        dataSource.serviceForSource(activeSource).associateWidget(progress, "progress");
    }
    
    layout: GraphicsLayouts.QGraphicsLinearLayout {

      
        PlasmaWidgets.IconWidget {
            id: previous
            Component.onCompleted: {
                setIcon("network-connect");
            }
            onClicked: {
	      var data = dataSource.serviceForSource('Hosts').operationDescription('Connect');
	      data.id = 0
              dataSource.serviceForSource('Hosts').startOperationCall(data);
            }
        }

//         PlasmaWidgets.Label {
// 	    text: "blabla"
// 	}

    }
}
