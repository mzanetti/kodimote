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
import org.kde.plasma.components 0.1
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts


Item {
    id: root
    property string activeSource: ''
    height: dataColumn.height
    width: height * 3

    property variant activePlayer: dataSource.data['Player']

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
            progress.value = data['Player'].percentage
        }
    }

    Component.onCompleted:
    {
        dataSource.serviceForSource(activeSource).associateWidget(stop, "stop");
        dataSource.serviceForSource(activeSource).associateWidget(progress, "progress");
    }

    Row {
        anchors.fill: parent

        Image {
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            width: height

            source: activePlayer.thumbnail
        }

        Column {
            id: dataColumn
            width: parent.width - x
            height: childrenRect.height

            Label {
                text: activePlayer.title
                anchors { left: parent.left; right: parent.right }
            }
            Label {
                text: "Year: " + activePlayer.year
                anchors { left: parent.left; right: parent.right }
            }
            Label {
                text: "Rating: " + activePlayer.rating
                anchors { left: parent.left; right: parent.right }
            }
            PlayerControls {
                anchors { left: parent.left; right: parent.right }
                dataSource: dataSource
            }

            Slider {
                id: progress
                anchors { left: parent.left; right: parent.right }
                maximumValue: 100
                value: activePlayer.percentage
            }
        }


    }



//        PlasmaWidgets.IconWidget {
//            id: previous
//            Component.onCompleted: {
//                setIcon("media-skip-backward");
//            }
//            onClicked: {
//                var data;
//                if(dataSource.data['Xbmc'].state == "video") {
//                    data = dataSource.serviceForSource('Player').operationDescription('SeekBackward');
//                } else {
//                    data = dataSource.serviceForSource('Player').operationDescription('SkipPrevious');
//                }

//                dataSource.serviceForSource('Player').startOperationCall(data);
//            }
//        }

//        PlasmaWidgets.IconWidget {
//            id: playPause

//            Component.onCompleted: {
//                setIcon("media-playback-start");
//            }
//            onClicked: {
//                var data = dataSource.serviceForSource('Player').operationDescription('PlayPause');

//                dataSource.serviceForSource('Player').startOperationCall(data);
//            }
//        }

//        PlasmaWidgets.IconWidget {
//            id: next
//            Component.onCompleted: {
//                setIcon("media-skip-forward");
//            }
//            onClicked: {
//                var data;
//                if(dataSource.data['Xbmc'].state == "video") {
//                    data = dataSource.serviceForSource('Player').operationDescription('SeekForward');
//                } else {
//                    data = dataSource.serviceForSource('Player').operationDescription('SkipNext');
//                }

//                dataSource.serviceForSource('Player').startOperationCall(data);
//            }
//        }

//        PlasmaWidgets.Slider {
//            id: progress
//            orientation: Qt.Horizontal

//            onSliderMoved: {
//                var operation = dataSource.serviceForSource('Xbmc').operationDescription("SetVolume");
//                operation.level = value

//                dataSource.serviceForSource('Xbmc').startOperationCall(operation);
//                print("set volume to " + value);
//            }
//        }
//    }
}
