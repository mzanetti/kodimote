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

FocusScope {
    id: contextMenu
    opacity: 0
    anchors.centerIn: parent
    width: 400; height: topImage.height + middleImage.height + bottomImage.height
    property alias model: contextMenuList.model

    function open() {
        forceActiveFocus();
    }

    signal accepted(int index)
    signal rejected()

    Keys.onPressed: {
        switch(event.key) {
        case Qt.Key_Down:
            contextMenuList.incrementCurrentIndex();
            break;
        case Qt.Key_Up:
            contextMenuList.decrementCurrentIndex();
            break;
        case Qt.Key_Escape:
        case Qt.Key_Backspace:
            contextMenu.rejected();
             break;
        case Qt.Key_Enter:
        case Qt.Key_Return:
            contextMenu.accepted(model.get(contextMenuList.currentIndex).entryId);
            break;
        }
        event.accepted = true;
    }

    MouseArea {
        anchors.fill: contextMenu
        onClicked: {
            console.log("yummi - Mousepresses taste delicious!")
        }
    }

    Image {
        id: topImage
        source: "images/DialogContextTop.png"
        anchors { left: parent.left; top: parent.top; right: parent.right }
    }
    Image {
        id: middleImage
        source: "images/DialogContextMiddle.png"
        anchors { left: parent.left; top: topImage.bottom; right: parent.right }
        height: contextMenu.model.count * 64
    }
    Image {
        id: bottomImage
        source: "images/DialogContextBottom.png"
        anchors { left: parent.left; bottom: parent.bottom; right: parent.right }
    }
    Image {
        id: closeButton
        source: closeButtonArea.pressed ? "images/DialogCloseButton-focus.png" : "images/DialogCloseButton.png"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 15
//            height: 20; width: 40

    }
    MouseArea {
        id: closeButtonArea
        anchors.fill: closeButton
        onClicked: contextMenu.rejected();
    }

    ListView {
        id: contextMenuList
        anchors.fill: middleImage
        anchors.margins: 5
        interactive: false
        currentIndex: 0

        delegate: Item {
            width: parent.width
            height: 64


            Image {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                source: index === contextMenuList.currentIndex ? "images/button-focus.png" : "images/button-nofocus.png"
            }
            Text {
                text: contextMenu.model.title(entryId)
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 28
            }
            MouseArea {
                id: delegateArea
                anchors.fill: parent
                onClicked: {
                    contextMenu.accepted(entryId);
                }
            }

        }
    }

    states: [
        State {
            name: "visible"; when: activeFocus
            PropertyChanges { target: contextMenu; opacity: 1 }
        }

    ]
}

