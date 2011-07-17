import QtQuick 1.1
import com.meego 1.0

Page {
    id: keyPad
    anchors.fill: parent
    tools: toolBarKeypad

    ToolBarLayout {
        id: toolBarKeypad
        visible: false
        ToolIcon { platformIconId: "toolbar-column";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                pageStack.pop();
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
                    source: "icons/button-arrow-up.png"
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
                    source: "icons/button-arrow-up.png"
                    rotation: -90
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.left();
                    }
                }
                Image {
                    id: okButton
                    source: "icons/button-ok.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.select();
                    }
                }
                Image {
                    id: arrowRight
                    source: "icons/button-arrow-up.png"
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
                    source: "icons/button-arrow-up.png"
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
                    source: "icons/button-back.png"
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
                    source: "icons/button-menu.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: keys.home();
                    }
                }
            }
        }
    }
}
