import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

Page {
    id: keyPad
    tools: toolBarKeypad
    orientationLock: PageOrientation.LockPortrait
    property bool keyboardOpen: false
    property QtObject player: xbmc.activePlayer

    onKeyboardOpenChanged: {
        tf.forceActiveFocus();
    }

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

    Column {
        width: parent.width - 30
        anchors.centerIn: parent
        spacing: 30

        GesturePad {
            id: gesturePad
            width: parent.width

            MediaControlButton {
                id: backButton
                anchors { left: parent.left; top: parent.top; margins: 10 }
                platformIconId: "toolbar-main-view"
                rotation: -90
                MouseArea {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: 55
                    onClicked: keys.back();
                }

            }

            MediaControlButton {
                anchors { right: parent.right; top: parent.top; margins: 10 }
                source: "icons/icon-m-toolbar-info.png"
                MouseArea {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: -145
                    onClicked: keys.info();
                }
            }
            MediaControlButton {
                anchors { left: parent.left; bottom: parent.bottom; margins: 10 }
                iconSource: "icon-m-image-edit-red-eyes-remove"
                MouseArea {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    rotation: -145
                    onClicked: keys.osd();
                }
            }
            MediaControlButton {
                anchors { right: parent.right; bottom: parent.bottom; margins: 10 }
                platformIconId: "toolbar-view-menu"
                MouseArea {
                    width: parent.width * 2
                    height: parent.height * 1.2
                    anchors.centerIn: parent
                    onClicked: keys.contextMenu();
                    rotation: 145
                }
            }
        }


        Item {
            width: parent.width
            height: buttonRow1.height
            Rectangle { anchors.fill: parent; opacity: theme.inverted ? 0.1 : 0.05; radius: 20; color: theme.inverted ? "white" : "black" }
            Grid {
                id: buttonRow1
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (width - fullscreenButton.width * 3) / 2
                columns: 3

                MediaControlButton {
                    id: fullscreenButton
                    platformIconId: "toolbar-main-view"
                    onClicked: keys.fullscreen();
                    rotation: 45
                }
                MediaControlButton {
                    platformIconId: "toolbar-home"
                    onClicked: keys.home();
                }
                MediaControlButton {
                    id: keyboardButton
                    platformIconId: "toolbar-grid"
                    onClicked: {
                        keyPad.keyboardOpen = true;
                        //                    var keyboard = keyboardComponent.createObject(keyPad);
                        //                    keyboard.open();
                    }
                }
            }
        }

        PlayerControls {
            id: controlButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            width: parent.width
            player: keyPad.player
            Rectangle { anchors.fill: controlButtons; opacity: theme.inverted ? 0.1 : 0.05; radius: 20; color: theme.inverted ? "white" : "black"; z: -1 }
        }

        Item {
            width: parent.width
            height: colorButtonRow.height
            Rectangle { anchors.fill: parent; opacity: theme.inverted ? 0.1 : 0.05; radius: 20; color: theme.inverted ? "white" : "black" }

            Row {
                id: colorButtonRow
                width: parent.width - spacing
                anchors.centerIn: parent
                height: controlButtons.height
                spacing: parent.width / 8
                Rectangle { height: 20; width: parent.spacing; color: "red"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                    MouseArea { anchors.fill: parent; onClicked: keys.red() }
                }
                Rectangle { height: 20; width: parent.spacing; color: "green"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                    MouseArea { anchors.fill: parent; onClicked: keys.green() }
                }
                Rectangle { height: 20; width: parent.spacing; color: "yellow"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                    MouseArea { anchors.fill: parent; onClicked: keys.yellow() }
                }
                Rectangle { height: 20; width: parent.spacing; color: "blue"; anchors.verticalCenter: parent.verticalCenter; radius: 2
                    MouseArea { anchors.fill: parent; onClicked: keys.blue() }
                }
            }
        }
    }

    SipAttributes {
        id: sipAttributes
        actionKeyIcon: "/opt/xbmcremote/qml/icons/closekeypad.svg"
    }

    TextField {
        id: tf
        anchors.top: parent.bottom
        inputMethodHints: Qt.ImhNoPredictiveText
        text: "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        property string oldText: "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        platformSipAttributes: sipAttributes

        Component.onCompleted: {
            tf.textChanged.connect(function() {
                print("text", text, text.charAt(text.length-1));
                if(text.length > oldText.length) {
                    keys.keyboardKey(tf.text.charAt(text.length-1));
                } else {
                    keys.backspace();
                }
                oldText = text;
            })
        }

        onActiveFocusChanged: {
            print("active focus changed:", keyPad.keyboardOpen, activeFocus);
            if(!activeFocus && keyPad.keyboardOpen) {
                focusTimer.start();
            }
        }

        Keys.onReturnPressed: {
            keyPad.keyboardOpen = false
            keyPad.focus = true;
            platformCloseSoftwareInputPanel()
        }

        Timer {
            id:focusTimer
            interval: 10
            repeat: false
            onTriggered: {
                if(keyPad.keyboardOpen) {
                    print("putting focus to input");
                    tf.forceActiveFocus();
                }
            }
        }
    }

}
