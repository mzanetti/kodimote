import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: keyPad
    tools: toolBarKeypad
    orientationLock: PageOrientation.LockPortrait
    property bool keyboardOpen: false

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
    property string orientation: width > height ? "landscape" : "portrait"

    Column {
        width: parent.width - 30
        anchors.centerIn: parent


        Grid {
            id: crossCol
            spacing: 50
            columns: 3
            anchors.horizontalCenter: parent.horizontalCenter
            KeypadButton {
                id: backButton
                icon: "back"
                onPressed: keys.back();
            }
            Rectangle {
                id: arrowUp
                width: backButton.width
                height: backButton.height
            }

            KeypadButton {
                icon: "info"
                onPressed: keys.info();
            }
            Rectangle {
                id: arrowLeft
                width: backButton.width
                height: backButton.height
            }

            Item {
                height: enter.height
                width: enter.width
                KeypadButton {
                    id: enter
                    icon: "enter"
                    states: [
                        State {
                            name: "moving"; when: moveArea.pressed
                            PropertyChanges {
                                target: enter
                                x: moveArea.mouseX - enter.width / 2
                                y: moveArea.mouseY - enter.height / 2
                            }
                        }

                    ]

                }
                MouseArea {
                    id: moveArea
                    anchors.fill: parent
                    anchors.margins: -30
                    property int startx
                    property int starty

                    onPressed: {
                        startx = mouseX
                        starty = mouseY
                    }

                    onClicked: {
                        keys.select();
                    }
                    onReleased: {
                        doPress();
                        repeatTimer.stop();
                    }
                    onPressAndHold: repeatTimer.start();

                    Timer {
                        id: repeatTimer
                        interval: 50
                        running: false;
                        repeat: true
                        onTriggered: {
                            moveArea.doPress();
                        }
                    }

                    function doPress() {
                        print(mouseX, startx);
                        print(mouseY, starty);
                        if(mouseY < starty - 100) {
                            keys.up();
                        } else if(mouseY > starty + 100) {
                            keys.down();
                        } else if(mouseX < startx - 100) {
                            keys.left();
                        } else if(mouseX > startx + 100) {
                            keys.right();
                        }
                    }
                }
            }

            Rectangle {
                id: arrowRight
                width: backButton.width
                height: backButton.height
            }
            KeypadButton {
                icon: "contextmenu"
                onPressed: keys.menu();
            }
            Rectangle {
                id: arrowDown
                width: backButton.width
                height: backButton.height
            }
            KeypadButton {
                icon: "menu"
                onPressed: keys.contextMenu();
            }
        }

        Item { width: parent.width; height: 50 }

        Grid {
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: (width - fullscreenButton.width * 3) / 2
            columns: 3
            visible: !keyPad.keyboardOpen

            KeypadButton {
                id: fullscreenButton
                icon: "fullscreen"
                onPressed: keys.fullscreen();
            }
            KeypadButton {
                icon: "home"
                onPressed: keys.home();
            }
            KeypadButton {
                id: keyboardButton
                icon: "keyboard"
                onPressed: {
                    keyPad.keyboardOpen = true;
//                    var keyboard = keyboardComponent.createObject(keyPad);
//                    keyboard.open();
                }
            }
        }
    }

    TextField {
        id: tf
        anchors.top: parent.bottom
        inputMethodHints: Qt.ImhNoPredictiveText
        text: "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        property string oldText: "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        onTextChanged: {
            print("text", text, text.charAt(text.length-1));
            if(text.length > oldText.length) {
                keys.keyboardKey(tf.text.charAt(text.length-1));
            } else {
                keys.backspace();
            }
            oldText = text;
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
