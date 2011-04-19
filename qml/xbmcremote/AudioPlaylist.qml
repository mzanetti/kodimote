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
//            Rectangle {
//                width: parent.width
//                height: 1
//                anchors.top: parent.bottom
//                color: "white"
//                opacity: 0.5

//            }
            Image {
                anchors.fill: parent
                source: AudioPlaylist.currentTrackNumber - 1 == index ? "images/MenuItemFO.png" : "images/MenuItemNF.png"
            }
            Image {
                anchors {top: parent.top; right: parent.right; bottom: parent.bottom }
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                source: "images/MediaItemDetailBG.png"
                visible: AudioPlaylist.currentTrackNumber - 1 == index
            }

            Text {
                color: "white"
                text: label
                font.pixelSize: 28
                anchors.fill: parent
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                color: "white"
                text: duration
                font.pixelSize: 28
                anchors { top: parent.top; right: parent.right; bottom: parent.bottom }
                anchors.rightMargin: 10
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
