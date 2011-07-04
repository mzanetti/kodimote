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
                spacing: 20
                columns: 3
                anchors.centerIn: parent
                Item {
                    width: arrowUp.width
                    height: arrowUp.height
                }
                Image {
                    id: arrowUp
                    source: "images/button-arrow-up.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.up();
                    }
                }
                Item {
                    width: arrowUp.width
                    height: arrowUp.height
                }
                Image {
                    id: arrowLeft
                    source: "images/button-arrow-up.png"
                    rotation: -90
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.left();
                    }
                }
                Image {
                    id: okButton
                    source: "images/button-ok.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.select();
                    }
                }
                Image {
                    id: arrowRight
                    source: "images/button-arrow-up.png"
                    rotation: 90
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.right();
                    }
                }
                Item {
                    width: arrowUp.width
                    height: arrowUp.height
                }
                Image {
                    id: arrowDown
                    source: "images/button-arrow-up.png"
                    rotation: 180
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.down();
                    }
                }
                Item {
                    width: arrowUp.width
                    height: arrowUp.height
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
                    source: "images/button-back.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.back();
                    }
                }
                Item {
                    width: homeButton.width
                    height: homeButton.height
                }

                Image {
                    id: homeButton
                    source: "images/button-menu.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.home();
                    }
                }
            }
        }
    }
}
