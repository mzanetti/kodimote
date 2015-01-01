/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import Qt 4.7

Item {
    anchors.fill: parent
    ListModel {
        id: controlsModel
        ListElement { buttonId: 0; imageFile: "images/media-skip-backward.png" }
        ListElement { buttonId: 1; imageFile: "images/media-seek-backward.png" }
        ListElement { buttonId: 2; imageFile: "images/media-playback-start.png" }
        ListElement { buttonId: 2; imageFile: "images/media-playback-stop.png" }
        ListElement { buttonId: 3; imageFile: "images/media-seek-forward.png" }
        ListElement { buttonId: 4; imageFile: "images/media-skip-forward.png" }
    }

    ListView {
        id: videoControls
        model: controlsModel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        orientation: ListView.Horizontal
        height:  50
        delegate: Item {
            id: myDelegate
            height: parent.height
            width: parent.width / videoControls.count
            Rectangle {
                color: "red"
                border.color: "black"
                anchors.fill: myDelegate
            }
            Image {
                source: imageFile
                anchors.centerIn: myDelegate
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    switch(buttonId) {
                    case 0:
                        VideoPlayer.skipPrevious()
                        break;
                    case 1:
                        VideoPlayer.play()
                        break;
                    case 2:
                        VideoPlayer.skipNext()
                        break;
                    case 3:
                        VideoPlayer.play()
                        break;
                    }
                }
            }
        }
    }
}
