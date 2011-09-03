import QtQuick 1.0

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

    Grid {
        anchors.fill: parent
        anchors.margins: 30
        columns: orientation == "portrait" ? 1 : 3

        Item {
            anchors.left: parent.left
            anchors.top: parent.top
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
                    source: "images/up_u.png"
                    MouseArea {
                        id: upMouseArea
                        anchors.fill: parent
                        onClicked: keys.up();
                    }
                    states: [
                        State {
                            when: upMouseArea.pressed;
                            PropertyChanges { target: arrowUp; source: "images/up_d.png" }
                        }
                    ]
                }
                Item {
                    width: arrowRight.width
                    height: arrowUp.height
                }
                Image {
                    id: arrowLeft
                    source: "images/left_u.png"
                    MouseArea {
                        id: leftMouseArea
                        anchors.fill: parent
                        onClicked: keys.left();
                    }
                    states: [
                        State {
                            when: leftMouseArea.pressed;
                            PropertyChanges { target: arrowLeft; source: "images/left_d.png" }
                        }
                    ]
                }
                Item {
                    height: arrowLeft.height
                    width: arrowUp.width
                    Image {
                        anchors.centerIn: parent
                        id: okButton
                        source: "images/middle_u.png"
                        MouseArea {
                            id: okMouseArea
                            anchors.fill: parent
                            onClicked: keys.select();
                        }
                        states: [
                            State {
                                when: okMouseArea.pressed;
                                PropertyChanges { target: okButton; source: "images/middle_d.png" }
                            }
                        ]
                    }
                }
                Image {
                    id: arrowRight
                    source: "images/right_u.png"
                    MouseArea {
                        id: rightMouseArea
                        anchors.fill: parent
                        onClicked: keys.right();
                    }
                    states: [
                        State {
                            when: rightMouseArea.pressed;
                            PropertyChanges { target: arrowRight; source: "images/right_d.png" }
                        }
                    ]
                }
                Item {
                    width: arrowLeft.width
                    height: arrowDown.height
                }
                Image {
                    id: arrowDown
                    source: "images/down_u.png"
                    MouseArea {
                        id: downMouseArea
                        anchors.fill: parent
                        onClicked: keys.down();
                    }
                    states: [
                        State {
                            when: downMouseArea.pressed;
                            PropertyChanges { target: arrowDown; source: "images/down_d.png" }
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
            Grid {
                anchors.centerIn: parent
                id: buttonRowGrid
                spacing: 20
                columns: orientation == "portrait" ? 3 : 1
                Image {
                    id: backButton
                    source: "images/back_u.png"
                    MouseArea {
                        id: backMouseArea
                        anchors.fill: parent
                        onClicked: keys.back();
                    }
                    states: [
                        State {
                            when: backMouseArea.pressed;
                            PropertyChanges { target: backButton; source: "images/back_d.png" }
                        }
                    ]
                }
                Item {
                    width: homeButton.width
                    height: homeButton.height
                }

                Image {
                    id: homeButton
                    source: "images/button2_u.png"
                    MouseArea {
                        id: homeMouseArea
                        anchors.fill: parent
                        onClicked: keys.home();
                    }
                    states: [
                        State {
                            when: homeMouseArea.pressed;
                            PropertyChanges { target: homeButton; source: "images/button2_d.png" }
                        }
                    ]
                }
            }
        }
    }
}
