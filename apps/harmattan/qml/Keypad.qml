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


        GesturePad {
            id: gesturePad
            width: parent.width

            MediaControlButton {
                id: backButton
                anchors { left: parent.left; top: parent.top; margins: 10 }
                platformIconId: "toolbar-main-view"
                onClicked: keys.back();
                rotation: -90
                showBorder: true
            }

            MediaControlButton {
                anchors { right: parent.right; top: parent.top; margins: 10 }
                text: "i"
                onClicked: keys.info();
                showBorder: true
            }
            MediaControlButton {
                anchors { left: parent.left; bottom: parent.bottom; margins: 10 }
                iconSource: "icon-m-image-edit-red-eyes-remove"
                onClicked: keys.osd();
                showBorder: true
            }
            MediaControlButton {
                anchors { right: parent.right; bottom: parent.bottom; margins: 10 }
                platformIconId: "toolbar-view-menu"
                onClicked: keys.contextMenu();
                showBorder: true
            }
        }

        Item { width: parent.width; height: 50 }

        Grid {
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: (width - fullscreenButton.width * 3) / 2
            columns: 3
            visible: !keyPad.keyboardOpen

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

        PlayerControls {
            id: controlButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            player: keyPad.player
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

    Timer {
        id: holdAndReleaseTimer
        interval: 50
        repeat: true
        running: false
        property int key: -1

        onTriggered: {
            switch(key) {
            case 0:
                keys.up();
                break;
            case 1:
                keys.down();
                break;
            case 2:
                keys.left();
                break;
            case 3:
                keys.right();
                break;
            }
        }
    }
}
