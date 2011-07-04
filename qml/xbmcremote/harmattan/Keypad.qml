import QtQuick 1.1
import com.meego 1.0

Page {
    id: keyPad
    anchors.fill: parent
    tools: toolBarBack

    property QtObject keys: xbmc.keys()

    Column {
        width: (arrowUp.width + spacing) * 3
        anchors.centerIn: parent
        spacing: 20
        Row {
            width: parent.width
            Image {
                id: arrowUp
                source: "icons/button-arrow-up.png"
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.up();
                }
            }
        }
        Row {
            width: parent.width
            spacing: 20
            Image {
                id: arrowLeft
                source: "icons/button-arrow-up.png"
                rotation: -90
                anchors.left: parent.left
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.left();
                }
            }
            Image {
                id: okButton
                source: "icons/button-arrow-up.png"
                rotation: 90
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.select();
                }
            }
            Image {
                id: arrowRight
                source: "icons/button-arrow-up.png"
                rotation: 90
                anchors.right: parent.right
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.right();
                }
            }

        }
        Row {
            width: parent.width
            Image {
                id: arrowDown
                source: "icons/button-arrow-up.png"
                rotation: 180
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.down();
                }
            }
        }
        Row {
            width: parent.width
            spacing: 20
            Image {
                id: backButton
                source: "icons/button-arrow-up.png"
                anchors.left: parent.left
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.back();
                }
            }
            Image {
                id: homeButton
                source: "icons/button-arrow-up.png"
                anchors.right: parent.right
                MouseArea {
                    anchors.fill: parent
                    onClicked: keys.home();
                }
            }
        }
    }

}
