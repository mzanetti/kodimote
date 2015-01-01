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

    property QtObject keys: kodi.keys()
    property string orientation: width > height ? "landscape" : "portrait"

    function keyPressed(event) {
        switch(event.key) {
        case Qt.Key_Left:
            keys.left();
            break;
        case Qt.Key_Right:
            keys.right();
            break;
        case Qt.Key_Up:
            keys.up();
            break;
        case Qt.Key_Down:
            keys.down();
            break;
        case Qt.Key_Return:
        case Qt.Key_Enter:
//        case Qt.Key_Space:
            keys.select();
            break;
//        case Qt.Key_Back:
        case Qt.Key_Backspace:
            keys.backspace();
            break;
        case Qt.Key_Escape:
            keys.back();
            break;
//        case Qt.Key_M:
//            keys.menu();
//            break;
//        case Qt.Key_H:
//        case Qt.Key_Home:
//            keys.home();
//            break;
//        case Qt.Key_C:
//            keys.contextMenu();
//            break;
//        case Qt.Key_I:
//            keys.info();
//            break;
//        case Qt.Key_F:
//            keys.fullscreen();
        default:
            keys.keyboardKey(event.text);
            break;
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
                    onPressed: keys.up();
                }
                Item {
                    width: arrowRight.width
                    height: arrowUp.height
                }
                KeypadButton {
                    id: arrowLeft
                    icon: "left"
                    onPressed: keys.left();
                }
                Item {
                    height: arrowLeft.height
                    width: arrowUp.width
                    KeypadButton {
                        anchors.centerIn: parent
                        id: okButton
                        icon: "enter"
                        onPressed: keys.select();
                    }
                }
                KeypadButton {
                    id: arrowRight
                    icon: "right"
                    onPressed: keys.right();
                }
                Item {
                    width: arrowLeft.width
                    height: arrowDown.height
                }
                KeypadButton {
                    id: arrowDown
                    icon: "down"
                    onPressed: keys.down();
                }
                Item {
                    width: arrowRight.width
                    height: arrowDown.height
                }
            }
        }
        Item {
            width: orientation == "portrait" ? parent.width : parent.width - crossCol.width - buttonRow.width
            height: orientation == "portrait" ? 200 : parent.height
//            Rectangle {color: "red"; anchors.fill: parent }

            KeypadButton {
                id: homeButton
                icon: "home"
                anchors.centerIn: parent
                onPressed: keys.home();
            }

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
                columns: 2
                KeypadButton {
                    id: backButton
                    icon: "back"
                    onPressed: keys.back();
                }
                KeypadButton {
                    id: infoButton
                    icon: "info"
                    onPressed: keys.info();
                }
                KeypadButton {
                    id: contextMenuButton
                    icon: "menu"
                    onPressed: {
                        keys.osd();
                        keys.contextMenu();
                    }
                }
                KeypadButton {
                    id: fullscreenButton
                    icon: "fullscreen"
                    onPressed: keys.fullscreen();
                }
            }
        }
    }
}
