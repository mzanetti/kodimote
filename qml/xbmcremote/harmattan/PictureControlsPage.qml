import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: pictureControls
    tools: toolBarPictureControls
    orientationLock: PageOrientation.LockPortrait

    ToolBarLayout {
        id: toolBarPictureControls
        ToolIcon { platformIconId: "toolbar-column";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
            }
        }
        ToolIcon { platformIconId: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                pageStack.replace(keypadPage);
            }
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.replace(nowPlayingPage)
        }
    }

    property QtObject player: xbmc.picturePlayer()
    property string orientation: width > height ? "landscape" : "portrait"

    Column {
        width: parent.width - 30
        anchors.centerIn: parent

        Row {
            width: parent.width
            spacing: (width - minusButton.width * 3) / 2

            KeypadButton {
                id: minusButton
                icon: "minus"
                onPressed: player.zoomOut();
            }
            Item {width: minusButton.width; height: minusButton.height}
            KeypadButton {
                icon: "plus"
                onPressed: player.zoomIn();
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
                mouseAreaTopMargin: -70
                onPressed: player.moveUp();
            }

            Item {
                width: arrowRight.width
                height: arrowUp.height
            }
            KeypadButton {
                id: arrowLeft
                icon: "left"
                mouseAreaLeftMargin: -100
                onPressed: player.moveLeft();
            }

            KeypadButton {
                id: rotateButton
                anchors.centerIn: parent
                icon: "rotate"
                onPressed: player.rotate();
            }
            KeypadButton {
                id: arrowRight
                icon: "right"
                mouseAreaRightMargin: -100
                onPressed: player.moveRight();
            }
            Item {
                width: arrowLeft.width
                height: arrowDown.height
            }
            KeypadButton {
                id: arrowDown
                icon: "down"
                mouseAreaBottomMargin: -50
                onPressed: player.moveDown();
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
                icon: "rewind"
                onPressed: player.skipPrevious();
            }
            KeypadButton {
                icon: "forward"
                onPressed: player.skipNext();
            }
            KeypadButton {
                icon: "playpause"
                onPressed: player.playPause();
            }
            KeypadButton {
                icon: "stop"
                onPressed: player.stop();
            }
        }
    }
}
