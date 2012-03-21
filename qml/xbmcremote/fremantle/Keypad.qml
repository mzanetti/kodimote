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

    property QtObject keys: xbmc.keys()
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
        case Qt.Key_Space:
            keys.select();
            break;
        case Qt.Key_Back:
        case Qt.Key_Backspace:
        case Qt.Key_Escape:
            keys.back();
            break;
        case Qt.Key_M:
            keys.menu();
            break;
        case Qt.Key_H:
        case Qt.Key_Home:
            keys.home();
            break;
        case Qt.Key_C:
            keys.contextMenu();
            break;
        case Qt.Key_I:
            keys.info();
            break;
        case Qt.Key_F:
            keys.fullscreen();
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
                columns: orientation == "portrait" ? 2 : 3
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
                    id: menuButton
                    icon: "contextmenu"
                    onPressed: keys.menu();
                }
                KeypadButton {
                    id: contextMenuButton
                    icon: "menu"
                    onPressed: keys.contextMenu();
                }
                KeypadButton {
                    id: homeButton
                    icon: "home"
                    onPressed: keys.home();
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
