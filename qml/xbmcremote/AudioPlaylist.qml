import QtQuick 1.0

Rectangle {
    anchors.fill: parent
    anchors.margins: 10
    color: "black"
    border.color: "white"
    radius: 10
    opacity: 0.8

    property alias model: list.model

    onVisibleChanged: {
        console.log("mdoelchanged")
    }

    Image {
        anchors.fill: parent
        source: "images/GlassOverlay.png"
    }

    ListView {
        id: list
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
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
         anchors.topMargin: 20
         anchors.bottomMargin: 20
     }

}
