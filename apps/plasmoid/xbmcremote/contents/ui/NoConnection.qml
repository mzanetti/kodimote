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
