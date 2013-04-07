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
    property int minimumHeight: mainColumn.height + spacing * 2
    property int minimumWidth: height * 2

    property int spacing: headerLabel.height / 2

    PlasmaCore.DataSource {
        id: dataSource
        dataEngine: "xbmcremote"
        connectedSources: ['Xbmc']
        //interval: 5000

        onNewData: {
            print("PopupDialog.qml: got new data", data['Xbmc'])
            if (data.connected) {
                listView.currentIndex = 1
            } else {
                listView.currentIndex = 0
            }
        }

        onDataChanged: {
            volumeSlider.value = data['Xbmc'].volume
        }
    }

    Column {
        id: mainColumn
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: root.spacing
        }
        height: childrenRect.height
        spacing: root.spacing


        Item {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: headerLabel.height

            Label {
                id: headerLabel
                text: qsTr("Xbmc on %1").arg(dataSource.data['Xbmc'].hostName)
                anchors {
                    left: parent.left
                    right: volumeRow.left
                    verticalCenter: parent.verticalCenter
                }
                elide: Text.ElideRight
                opacity: listView.currentIndex
                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }
            }
            Label {
                anchors {
                    left: parent.left
                    right: volumeRow.left
                    verticalCenter: parent.verticalCenter
                }
                text: qsTr("Connect to...")
                opacity: 1 - listView.currentIndex
                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }
            }

            Row {
                id: volumeRow
                anchors.right: parent.right
                height: parent.height
                spacing: root.spacing / 2
                opacity: listView.currentIndex

                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }

                PlasmaCore.IconItem {
                    width: theme.iconSizes.small
                    height: theme.iconSizes.small
                    anchors.verticalCenter: parent.verticalCenter
                    source: "audio-volume-low"
                    MouseArea { anchors.fill: parent;
                        onClicked: {
                            var data = dataSource.serviceForSource('Xbmc').operationDescription("SetVolume")
                            data.level = dataSource.data['Xbmc'].volume - 5
                            dataSource.serviceForSource('Xbmc').startOperationCall(data)
                        }
                    }
                }
                Slider {
                    id: volumeSlider
                    orientation: Qt.Horizontal
                    maximumValue: 100
                    width: root.spacing * 10
                    anchors.verticalCenter: parent.verticalCenter
                    stepSize: 5
                    onValueChanged: {
                        if (pressed) {
                            var data = dataSource.serviceForSource('Xbmc').operationDescription("SetVolume")
                            data.level = value
                            dataSource.serviceForSource('Xbmc').startOperationCall(data)
                        }
                    }
                }
                PlasmaCore.IconItem {
                    width: theme.iconSizes.small
                    height: theme.iconSizes.small
                    anchors.verticalCenter: parent.verticalCenter
                    source: "audio-volume-high"
                    MouseArea { anchors.fill: parent;
                        onClicked: {
                            var data = dataSource.serviceForSource('Xbmc').operationDescription("SetVolume")
                            data.level = dataSource.data['Xbmc'].volume + 5
                            dataSource.serviceForSource('Xbmc').startOperationCall(data)
                        }
                    }
                }
                PlasmaCore.IconItem {
                    width: theme.iconSizes.small
                    height: theme.iconSizes.small
                    anchors.verticalCenter: parent.verticalCenter
                    source: "window-close"
                    MouseArea { anchors.fill: parent;
                        onClicked: {
                            listView.currentIndex = 0
                        }
                    }
                }
            }
        }

        VisualItemModel {
            id: visualItemModel
            Dummy {
                height: listView.height
                width: listView.width
                discovering: listView.currentIndex == 0
            }

            NowPlaying {
                id: nowPlaying
                spacing: root.spacing
                width: listView.width
                height: listView.height
            }
//            ListView {
//                width: listView.width
//                height: listView.height
//                model: PlasmaCore.DataModel {
//                    dataSource: dataSource
//                    keyRoleFilter: "MainMenu[\\d]*"
//                }

//                delegate: Rectangle {
//                    width: parent.width
//                    height: 50
//                    color: "blue"
//                    Text {
//                        anchors.centerIn: parent
//                        text: title
//                    }
//                }
//            }
        }

        ListView {
            id: listView
            model: visualItemModel
            anchors {
                left: parent.left
                right: parent.right
            }
            height: nowPlaying.minimumHeight
            orientation: ListView.Horizontal
            clip: true
            interactive: false
            highlightMoveDuration: 200
        }
    }
}
