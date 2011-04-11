import QtQuick 1.0

Item {
//    anchors.fill: parent
    id: container

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 1
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        color: "white"
    }


//    gradient: Gradient {
//        GradientStop {
//            position: 1.00;
//            color: "#000000";
//        }
//        GradientStop {
//            position: 0.00;
//            color: "#979797";
//        }
//    }

    ListModel {
        id: controlsModel
        ListElement { buttonId: 0; imageFile: "images/OSDPrevTrackNF.png" }
        ListElement { buttonId: 1; imageFile: "images/OSDRewindNF.png" }
        ListElement { buttonId: 2; imageFile: "images/OSDPlayNF.png" }
        ListElement { buttonId: 3; imageFile: "images/OSDStopNF.png" }
        ListElement { buttonId: 4; imageFile: "images/OSDForwardNF.png" }
        ListElement { buttonId: 5; imageFile: "images/OSDNextTrackNF.png" }
    }

    ListView {
        id: audioControls
        model: controlsModel
        orientation: ListView.Horizontal
        anchors.fill: container

        delegate: Item {
            id: myDelegate
            height: parent.height
            width: container.width / audioControls.count
//            Rectangle {
//                color: "black"
//                border.color: "black"
//                anchors.fill: myDelegate
//            }
            Image {
                source: imageFile
                anchors.centerIn: myDelegate
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    switch(buttonId) {
                    case 0:
                        AudioPlayer.skipPrevious()
                        break;
                    case 1:
                        AudioPlayer.seekBackward()
                        break;
                    case 2:
                        AudioPlayer.playPause()
                        break;
                    case 3:
                        AudioPlayer.stop()
                        break;
                    case 4:
                        AudioPlayer.seekForward()
                        break;
                    case 5:
                        AudioPlayer.skipNext()
                        break;
                    }
                }
            }
        }
    }
}
