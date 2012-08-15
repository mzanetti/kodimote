import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

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

    Column {
        width: parent.width - 30
        anchors.centerIn: parent


        Image {
            id: gridbg
            source: "icons/pad-bg" + areaStateToString(gestureHelper.currentState) + ".png"

            states: [
                State {
                    when: arrowUpMouseArea.pressed
                    PropertyChanges { target: gridbg; source: "icons/pad-bg-up.png" }
                },
                State {
                    when: arrowLeftMouseArea.pressed
                    PropertyChanges { target: gridbg; source: "icons/pad-bg-left.png" }
                },
                State {
                    when: arrowRightMouseArea.pressed
                    PropertyChanges { target: gridbg; source: "icons/pad-bg-right.png" }
                },
                State {
                    when: arrowDownMouseArea.pressed
                    PropertyChanges { target: gridbg; source: "icons/pad-bg-down.png" }
                }

            ]

            anchors.horizontalCenter: parent.horizontalCenter
            function areaStateToString(areaState) {
                if(areaState === GestureHelper.AreaTop) {
                    return "-up";
                }
                if(areaState === GestureHelper.AreaLeft) {
                    return "-left";
                }
                if(areaState === GestureHelper.AreaRight) {
                    return "-right";
                }
                if(areaState === GestureHelper.AreaBottom) {
                    return "-down";
                }
                return "";
            }

            Grid {
                id: crossCol
                spacing: 50
                columns: 3
                anchors.centerIn: parent
                KeypadButton {
                    id: backButton
                    icon: "back"
                    onPressed: keys.back();
                }
                Item {
                    id: arrowUp
                    width: backButton.width
                    height: backButton.height
                    MouseArea {
                        id: arrowUpMouseArea
                        anchors.fill: parent
                        enabled: settings.gesturePadClickable
                        onClicked: {
                            keys.up();
                        }
                    }
                }

                KeypadButton {
                    icon: "info"
                    onPressed: keys.info();
                }
                Item {
                    id: arrowLeft
                    width: backButton.width
                    height: backButton.height
                    MouseArea {
                        id: arrowLeftMouseArea
                        anchors.fill: parent
                        enabled: settings.gesturePadClickable
                        onClicked: {
                            keys.left();
                        }
                    }
                }

                Item {
                    id: joystickButton
                    height: enter.height
                    width: enter.width

                    property int allowedArea: 26

                    GestureHelper {
                        id: gestureHelper
                    }

                    states: [
                        State {
                            name: "moving"; when: moveArea.pressed && !settings.gesturePadClickable
                            PropertyChanges {
                                target: enter
                                x: gestureHelper.restrictedX - moveArea.width / 2
                                y: gestureHelper.restrictedY - moveArea.height / 2
                            }
                        }

                    ]
                    KeypadButton {
                        id: enter
                        icon: "enter"

                    }
                    MouseArea {
                        id: moveArea
                        anchors.fill: parent
                        anchors.margins: -30

//                        Rectangle {
//                            anchors.fill: parent
//                        }

                        onPressed: {
                            if(!settings.gesturePadClickable) {
                                gestureHelper.startX = mouseX
                                gestureHelper.startY = mouseY
                            }
                        }

                        onMouseXChanged: {
                            if(!settings.gesturePadClickable) {
                                gestureHelper.realX = mouseX
                            }
                        }
                        onMouseYChanged: {
                            if(!settings.gesturePadClickable) {
                                gestureHelper.realY = mouseY
                            }
                        }

                        onClicked: {
                            rumbleEffect.start();
                            keys.select();
                        }
                        onReleased: {
                            if(repeatTimer.running) {
                                repeatTimer.stop();
                            } else {
                                rumbleEffect.start();
                                doPress();
                            }
                            gestureHelper.reset();
                        }
                        onPressAndHold: {
                            rumbleEffect.start(3);
                            repeatTimer.start();
                        }

                        Timer {
                            id: repeatTimer
                            interval: 2000
                            running: false;
                            repeat: true
                            property int newInterval: 2000 / (gestureHelper.depth == 0 ? 1 : gestureHelper.depth / 5)
                            onTriggered: {
                                print("interval:", interval)
                                moveArea.doPress();
                                interval = newInterval;
                            }
                        }

                        function doPress() {
                            switch(gestureHelper.currentState) {
                            case GestureHelper.AreaTop:
                                keys.up();
                                break;
                            case GestureHelper.AreaLeft:
                                keys.left();
                                break;
                            case GestureHelper.AreaRight:
                                keys.right();
                                break;
                            case GestureHelper.AreaBottom:
                                keys.down();
                                break;
                            }
                        }
                    }
                }
                Item {
                    id: arrowRight
                    width: backButton.width
                    height: backButton.height
                    MouseArea {
                        id: arrowRightMouseArea
                        anchors.fill: parent
                        enabled: settings.gesturePadClickable
                        onClicked: {
                            keys.right();
                        }
                    }
                }
                KeypadButton {
                    icon: "contextmenu"
                    onPressed: keys.menu();
                }
                Item {
                    id: arrowDown
                    width: backButton.width
                    height: backButton.height
                    MouseArea {
                        id: arrowDownMouseArea
                        anchors.fill: parent
                        enabled: settings.gesturePadClickable
                        onClicked: {
                            keys.down();
                        }
                    }
                }
                KeypadButton {
                    icon: "menu"
                    onPressed: keys.contextMenu();
                }
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

    SipAttributes {
        id: sipAttributes
        actionKeyIcon: "/opt/xbmcremote/qml/harmattan/icons/closekeypad.svg"
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
