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
  anchors.fill: parent
      ListModel {
	  id: hostModel
	  ListElement { modelData: "aaa" }
	  ListElement { modelData: "bbb" }
      }
      
      
               PlasmaCore.DataSource {
            id: xbmcSource
            dataEngine: "xbmcremote"
            connectedSources: ['Hosts']
            interval: 2000

	    onNewData: {
	      hostListView.model = xbmcSource.data['Hosts'].hostList
            }

            onDataChanged: {
	      hostListView.model = xbmcSource.data['Hosts'].hostList
            }
        }
        
        PlasmaCore.Theme {
                id: theme
        }

      
      PlasmaCore.DataModel {
	id: hostModel1
	dataSource: xbmcSource
//	sourceFilter: "Hosts"
//	keyRoleFilter: "name"
	
      }

      ListView {
	id: hostListView
	anchors.fill: parent
	model: xbmcSource.data['Hosts'].hostList
	
	delegate: PlasmaComponents.ListItem {
	  height: 50
	  width: parent.width
	  PlasmaComponents.Label {
	    id: hostLabel
	    anchors.fill: parent
	    text: modelData
	    width: parent.width
	  }
	  
	  MouseArea {
	      anchors.fill: parent
	      onClicked: {
		var data = dataSource.serviceForSource('Hosts').operationDescription('Connect');
		data.id = index
		dataSource.serviceForSource('Hosts').startOperationCall(data);
	      }
	  }
	}
      }
}
