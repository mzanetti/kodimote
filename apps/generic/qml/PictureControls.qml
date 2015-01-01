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

BorderImage {
    id: keyPad
    border.top: 15
    border.right: 15
    border.left: 15
    border.bottom: 15
    anchors.margins: 10
    anchors.fill: parent
    source: "images/ContentPanel.png"

    property QtObject player: kodi.picturePlayer()
    property string orientation: width > height ? "landscape" : "portrait"

    function keyPressed(event) {
        if(event.modifiers === Qt.ShiftModifier) {
            switch(event.key) {
            case Qt.Key_Left:
                player.skipPrevious();
                break;
            case Qt.Key_Right:
                player.skipNext();
                break;
            case Qt.Key_Up:
                player.zoomIn();
                break;
            case Qt.Key_Down:
                player.zoomOut();
                break;
            }
        } else {

            switch(event.key) {
            case Qt.Key_Left:
                player.moveLeft()
                break;
            case Qt.Key_Right:
                player.moveRight();
                break;
            case Qt.Key_Up:
                player.moveUp();
                break;
            case Qt.Key_Down:
                player.moveDown();
                break;
            case Qt.Key_Return:
            case Qt.Key_Enter:
            case Qt.Key_Space:
                player.playPause();
                break;
            case Qt.Key_Back:
            case Qt.Key_Backspace:
            case Qt.Key_Escape:
                player.stop();
                break;
            }
        }
    }

    Grid {
        anchors.fill: parent
        anchors.margins: 30
        columns: orientation == "portrait" ? 1 : 3

        Item {
            //            anchors.left: parent.left
            //            anchors.top: parent.top
            height: orientation == "portrait" ? crossCol.height : parent.height
            width: orientation == "portrait" ? parent.width : crossCol.width
            //            Rectangle { color: "green"; anchors.fill: parent }
            Grid {
                id: crossCol
                spacing: -30
                columns: 3
                anchors.centerIn: parent
                Item {
                    width: arrowLeft.width
                    height: arrowUp.height
                }
                KeypadButton {
                    id: arrowUp
                    icon: "up"
                    onPressed: player.moveUp();
                }
                Item {
                    width: arrowRight.width
                    height: arrowUp.height
                }
                KeypadButton {
                    id: arrowLeft
                    icon: "left"
                    onPressed: player.moveLeft();
                }
                Item {
                    height: arrowLeft.height
                    width: arrowUp.width
                    KeypadButton {
                        anchors.centerIn: parent
                        id: okButton
                        icon: "rotate"
                        onPressed: player.rotate();
                    }
                }
                KeypadButton {
                    id: arrowRight
                    icon: "right"
                    onPressed: player.moveRight();
                }
                Item {
                    width: arrowLeft.width
                    height: arrowDown.height
                }
                KeypadButton {
                    id: arrowDown
                    icon: "down"
                    onPressed: player.moveDown();
                }
                Item {
                    width: arrowRight.width
                    height: arrowDown.height
                }
            }
        }
        Item {
            width: orientation == "portrait" ? parent.width : parent.width - crossCol.width - buttonRow.width
            height: orientation == "portrait" ? 20 : parent.height
            //            Rectangle {color: "red"; anchors.fill: parent }
        }

        Item {
            id: buttonRow
            width: orientation == "portrait" ? parent.width : buttonRowGrid.width
            height: orientation == "portrait" ? buttonRowGrid.height : parent.height
            //            Rectangle {color: "blue"; anchors.fill: parent }
            Grid {
                anchors.centerIn: parent
                id: buttonRowGrid
                spacing: 20
                columns: orientation == "portrait" ? 2 : 4

                KeypadButton {
                    id: previousButton
                    icon: "rewind"
                    onPressed: player.skipPrevious();
                }
                KeypadButton {
                    id: nextButton
                    icon: "forward"
                    onPressed: player.skipNext();
                }
                KeypadButton {
                    id: playPauseButton
                    icon: "playpause"
                    onPressed: player.playPause();
                }
                KeypadButton {
                    id: stopButton
                    icon: "stop"
                    onPressed: player.stop();
                }
                Item {
                    width: zoomOutButton.width
                    height: zoomInButton.width
                    visible: orientation != "portrait"
                }

                KeypadButton {
                    id: zoomOutButton
                    icon: "minus"
                    onPressed: player.zoomOut();
                }
                KeypadButton {
                    id: zoomInButton
                    icon: "plus"
                    onPressed: player.zoomIn();
                }
            }
        }
    }
}
