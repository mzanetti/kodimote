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
            connectedSources: ['Hosts', 'Xbmc', 'Player']
            //interval: 5000

       onNewData: {
	      print("got new data", data[0])
            }

            onDataChanged: {
	      print("got new data", data['Xbmc'].volume)
              progress.value = data['Xbmc'].volume
	        if(dataSource.data['Player'].state == "playing") {
		  playPause.setIcon("media-playback-pause");
		} else {
		  playPause.setIcon("media-playback-start");
		}
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
                setIcon("media-skip-backward");
            }
            onClicked: {
	      var data;
		if(dataSource.data['Xbmc'].state == "video") {
		  data = dataSource.serviceForSource('Player').operationDescription('SeekBackward');
		} else {
		  data = dataSource.serviceForSource('Player').operationDescription('SkipPrevious');
		}

                dataSource.serviceForSource('Player').startOperationCall(data);
            }
        }

        PlasmaWidgets.IconWidget {
            id: playPause

            Component.onCompleted: {
		  setIcon("media-playback-start");
            }
            onClicked: {
                var data = dataSource.serviceForSource('Player').operationDescription('PlayPause');

                dataSource.serviceForSource('Player').startOperationCall(data);
            }
        }

        PlasmaWidgets.IconWidget {
            id: next
            Component.onCompleted: {
                setIcon("media-skip-forward");
            }
            onClicked: {
                var data;
		if(dataSource.data['Xbmc'].state == "video") {
		  data = dataSource.serviceForSource('Player').operationDescription('SeekForward');
		} else {
		  data = dataSource.serviceForSource('Player').operationDescription('SkipNext');
		}

                dataSource.serviceForSource('Player').startOperationCall(data);
            }
        }

        PlasmaWidgets.Slider {
            id: progress
            orientation: Qt.Horizontal

            onSliderMoved: {
                var operation = dataSource.serviceForSource('Xbmc').operationDescription("SetVolume");
                operation.level = value

                dataSource.serviceForSource('Xbmc').startOperationCall(operation);
                print("set volume to " + value);
            }
        }
    }
}
