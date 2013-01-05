
import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: root

    property int minimumWidth: l.item.minimumWidth
    property int minimumHeight: l.item.minimumHeight


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

      Loader {
	  id: l
	  source: {
  //             if (plasmoid.formFactor == Planar || plasmoid.formFactor == MediaCenter)
  //                 "FullLayout.qml"
  //             else 
	      if(dataSource.data['Xbmc'].connected) {
		if (plasmoid.formFactor == Horizontal) {
		    print("loading compact layout")
		    "HorizontalDock.qml"
		} else {
		    print("loading other layout")
		    "NowPlayingSmall.qml"
		}
	      } else {
		  "Dummy.qml"
	      }

	  }
	  
	  anchors.fill: parent
      }

//     Component.onCompleted: {
//         plasmoid.aspectRatioMode = plasmoid.IgnoreAspectRatio;
//        plasmoid.popupIcon = "konqueror"
//     }
}
