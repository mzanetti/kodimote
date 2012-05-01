import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: keyPad
    tools: toolBarKeypad
    orientationLock: PageOrientation.LockPortrait

    ToolBarLayout {
        id: toolBarKeypad
        ToolButton { iconSource: "icons/tools-columns.png";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
            }
        }
        ToolButton { iconSource: "toolbar-refresh";
             anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
             visible: xbmc.picturePlayerActive
             onClicked: {
                 pageStack.replace(pictureControlsPage);
             }
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.replace(nowPlayingPage)
        }
    }

    property QtObject keys: xbmc.keys()
    property string orientation: width > height ? "landscape" : "portrait"

    Column {
        id: buttonColumn
        width: parent.width - 30
        anchors.centerIn: parent
        property int buttonSize: 72

        Row {
            width: parent.width
            spacing: (width - backButton.width * 3) / 2

            KeypadButton {
                id: backButton
                icon: "back"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.back();
            }
            Item {width: backButton.width; height: backButton.height}
            KeypadButton {
                icon: "info"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.info();
            }
        }


        Grid {
            id: crossCol
            spacing: -20
            columns: 3
            anchors.horizontalCenter: parent.horizontalCenter
            Item {
                width: arrowLeft.width
                height: arrowUp.height
            }
            KeypadButton {
                id: arrowUp
                icon: "up"
                mouseAreaTopMargin: -50
                onPressed: keys.up();
            }

            Item {
                width: arrowRight.width
                height: arrowUp.height
            }
            KeypadButton {
                id: arrowLeft
                icon: "left"
                mouseAreaLeftMargin: -70
                onPressed: keys.left();
            }

            Item {
                width: arrowUp.width
                height: arrowLeft.height
                KeypadButton {
                    id: enter
                    anchors.centerIn: parent
                    icon: "enter"
                    onPressed: keys.select();
                }
            }
            KeypadButton {
                id: arrowRight
                icon: "right"
                mouseAreaRightMargin: -70
                onPressed: keys.right();
            }
            Item {
                width: arrowLeft.width
                height: arrowDown.height
            }
            KeypadButton {
                id: arrowDown
                icon: "down"
                mouseAreaBottomMargin: -50
                onPressed: keys.down();
            }
            Item {
                width: arrowRight.width
                height: arrowDown.height
            }
        }


        Item { width: parent.width; height: 30 }

        Grid {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            columns: 2

            KeypadButton {
                icon: "contextmenu"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.menu();
            }
            KeypadButton {
                icon: "menu"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.contextMenu();
            }
            KeypadButton {
                icon: "home"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.home();
            }
            KeypadButton {
                id: fullscreenButton
                icon: "fullscreen"
                height: buttonColumn.buttonSize
                width: buttonColumn.buttonSize
                onPressed: keys.fullscreen();
            }
        }
    }
}
