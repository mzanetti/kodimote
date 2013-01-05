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