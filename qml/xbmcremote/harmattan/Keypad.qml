import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: keyPad
    tools: toolBarKeypad
    orientationLock: PageOrientation.LockPortrait

    ToolBarLayout {
        id: toolBarKeypad
        ToolIcon { platformIconId: "toolbar-column";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
            }
        }
        ToolIcon { platformIconId: "toolbar-image-edit";
             anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
             visible: xbmc.picturePlayerActive
             onClicked: {
                 pageStack.replace(pictureControlsPage);
             }
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.replace(nowPlayingPage)
        }
    }

    property QtObject keys: xbmc.keys()
    property string orientation: width > height ? "landscape" : "portrait"

    Column {
        width: parent.width - 30
        anchors.centerIn: parent

        Row {
            width: parent.width
            spacing: (width - backButton.width * 3) / 2

            KeypadButton {
                id: backButton
                icon: "back"
                onPressed: keys.back();
            }
            Item {width: backButton.width; height: backButton.height}
            KeypadButton {
                icon: "info"
                onPressed: keys.info();
            }
        }


        Grid {
            id: crossCol
            spacing: -30
            columns: 3
            anchors.horizontalCenter: parent.horizontalCenter
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

            KeypadButton {
                id: enter
                anchors.centerIn: parent
                icon: "enter"
                onPressed: keys.select();
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

        Item { width: parent.width; height: 30 }

        Grid {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 50
            columns: 2

            KeypadButton {
                icon: "contextmenu"
                onPressed: keys.menu();
            }
            KeypadButton {
                icon: "menu"
                onPressed: keys.contextMenu();
            }
            KeypadButton {
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
