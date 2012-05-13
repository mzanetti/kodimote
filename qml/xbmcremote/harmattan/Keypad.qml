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
                mouseAreaTopMargin: -70
                onPressed: keys.up();
            }

            Item {
                width: arrowRight.width
                height: arrowUp.height
            }
            KeypadButton {
                id: arrowLeft
                icon: "left"
                mouseAreaLeftMargin: -100
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
                mouseAreaRightMargin: -100
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


        Row {
            width: parent.width
            spacing: (width - backButton.width * 3) / 2
            KeypadButton {
                icon: "contextmenu"
                onPressed: keys.menu();
            }
            Item {width: backButton.width; height: backButton.height}
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

//    Component {
//        id: keyboardComponent
//        Sheet {
//            acceptButtonText: qsTr("Close")

//            Component.onCompleted: tf.forceActiveFocus();

            TextField {
                id: tf
                anchors.top: parent.bottom
                inputMethodHints: Qt.ImhNoPredictiveText
                text: "x"
                property string oldText: "x"
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
                    onTriggered: {
                        if(keyPad.keyboardOpen) {
                            print("putting focus to input");
                            tf.forceActiveFocus();
                        }
                    }
                }
            }
//        }
//    }

}
