import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: pictureControls
    tools: toolBarPictureControls

    ToolBarLayout {
        id: toolBarPictureControls
        ToolButton { iconSource: "toolbar-column";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
            }
        }
        ToolButton { iconSource: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                pageStack.replace(keypadPage);
            }
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.replace(nowPlayingPage)
        }
    }

    property QtObject player: xbmc.picturePlayer()
    property string orientation: width > height ? "landscape" : "portrait"

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
                Image {
                    id: arrowUp
                    source: "icons/up_u.png"
                    MouseArea {
                        id: upMouseArea
                        anchors.fill: parent
                        onClicked: player.moveUp()
                    }
                    states: [
                        State {
                            when: upMouseArea.pressed;
                            PropertyChanges { target: arrowUp; source: "icons/up_d.png" }
                        }
                    ]
                }
                Item {
                    width: arrowRight.width
                    height: arrowUp.height
                }
                Image {
                    id: arrowLeft
                    source: "icons/left_u.png"
                    MouseArea {
                        id: leftMouseArea
                        anchors.fill: parent
                        onClicked: player.moveLeft()
                    }
                    states: [
                        State {
                            when: leftMouseArea.pressed;
                            PropertyChanges { target: arrowLeft; source: "icons/left_d.png" }
                        }
                    ]
                }
                Item {
                    height: arrowLeft.height
                    width: arrowUp.width
                    Image {
                        anchors.centerIn: parent
                        id: okButton
                        source: "icons/rotate_u.png"
                        MouseArea {
                            id: okMouseArea
                            anchors.fill: parent
                            onClicked: player.rotate();
                        }
                        states: [
                            State {
                                when: okMouseArea.pressed;
                                PropertyChanges { target: okButton; source: "icons/rotate_d.png" }
                            }
                        ]
                    }
                }
                Image {
                    id: arrowRight
                    source: "icons/right_u.png"
                    MouseArea {
                        id: rightMouseArea
                        anchors.fill: parent
                        onClicked: player.moveRight();
                    }
                    states: [
                        State {
                            when: rightMouseArea.pressed;
                            PropertyChanges { target: arrowRight; source: "icons/right_d.png" }
                        }
                    ]
                }
                Item {
                    width: arrowLeft.width
                    height: arrowDown.height
                }
                Image {
                    id: arrowDown
                    source: "icons/down_u.png"
                    MouseArea {
                        id: downMouseArea
                        anchors.fill: parent
                        onClicked: player.moveDown();
                    }
                    states: [
                        State {
                            when: downMouseArea.pressed;
                            PropertyChanges { target: arrowDown; source: "icons/down_d.png" }
                        }
                    ]
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
            Column {
                anchors.centerIn: parent
                id: buttonRowGrid
                spacing: 20

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Image {
                        id: previousButton
                        source: "icons/previous_u.png"
                        MouseArea {
                            id: previousMouseArea
                            anchors.fill: parent
                            onClicked: player.skipPrevious();
                        }
                        states: [
                            State {
                                when: previousMouseArea.pressed;
                                PropertyChanges { target: previousButton; source: "icons/previous_d.png" }
                            }
                        ]
                    }
                    Image {
                        id: nextButton
                        source: "icons/next_u.png"
                        MouseArea {
                            id: nextMouseArea
                            anchors.fill: parent
                            onClicked: player.skipNext();
                        }
                        states: [
                            State {
                                when: nextMouseArea.pressed;
                                PropertyChanges { target: nextButton; source: "icons/next_d.png" }
                            }
                        ]
                    }
                    Image {
                        id: playPauseButton
                        source: "icons/playpause_u.png"
                        MouseArea {
                            id: playPauseMouseArea
                            anchors.fill: parent
                            onClicked: player.playPause();
                        }
                        states: [
                            State {
                                when: playPauseMouseArea.pressed;
                                PropertyChanges { target: playPauseButton; source: "icons/playpause_d.png" }
                            }
                        ]
                    }
                }
                Row{
                    spacing: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    Image {
                        id: zoomOutButton
                        source: "icons/zoomOut_u.png"
                        MouseArea {
                            id: zoomOutMouseArea
                            anchors.fill: parent
                            onClicked: player.zoomOut();
                        }
                        states: [
                            State {
                                when: zoomOutMouseArea.pressed;
                                PropertyChanges { target: zoomOutButton; source: "icons/zoomOut_d.png" }
                            }
                        ]
                    }
                    Image {
                        id: zoomInButton
                        source: "icons/zoomIn_u.png"
                        MouseArea {
                            id: zoomInMouseArea
                            anchors.fill: parent
                            onClicked: player.zoomIn();
                        }
                        states: [
                            State {
                                when: zoomInMouseArea.pressed;
                                PropertyChanges { target: zoomInButton; source: "icons/zoomIn_d.png" }
                            }
                        ]
                    }
                    Image {
                        id: stopButton
                        source: "icons/stop_u.png"
                        MouseArea {
                            id: stopMouseArea
                            anchors.fill: parent
                            onClicked: {
                                player.stop();
                                pageStack.replace(keypadPage);
                            }
                        }
                        states: [
                            State {
                                when: stopMouseArea.pressed;
                                PropertyChanges { target: stopButton; source: "icons/stop_d.png" }
                            }
                        ]
                    }
                }
            }
        }
    }
}
