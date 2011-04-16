import QtQuick 1.0

BorderImage {
    border.top: 15
    border.right: 15
    border.left: 15
    border.bottom: 15
    anchors.margins: 10
    anchors.fill: parent
    source: "images/ContentPanel.png"

    property alias model: list.model

    ListView {
        id: list
        anchors.fill: parent
        anchors.margins: 25
        clip: true

        delegate: Item {
            width: parent.width
            height: 64
            Rectangle {
                width: parent.width
                height: 1
                anchors.top: parent.top
                color: "white"
                opacity: 0.5

            }

            Text {
                color: "white"
                text: label
                font.pixelSize: 28
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    list.model.playItem(index)
                }
            }
        }
    }

    ScrollBar {
         scrollArea: list; width: 15; alwaysVisible: false
         anchors { right: parent.right; top: parent.top; bottom: parent.bottom}
         anchors.rightMargin: 10
         anchors.topMargin: 20
         anchors.bottomMargin: 20
     }

}
