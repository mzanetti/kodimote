import QtQuick 1.0

Rectangle {
    anchors.fill: parent
    color: "black"
    state: AudioLibrary.state

    Rectangle {

        id: topMenu
        width: parent.width; height: 60
        anchors.top: parent.top
        color: "gray"
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#000000";
            }
            GradientStop {
                position: 1.00;
                color: "#979797";
            }
        }

        Text {
            id: locationText
            anchors.fill: parent
            anchors.margins: 20
            color: "#0084ff"
            font.pixelSize: 28
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    ListView {
        id: list
        anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: topMenu.bottom }
        anchors.margins: 20
//        anchors.rightMargin: 20
        clip: true
        model: AudioLibrary

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
                    list.model.enterItem(itemId)

                }
            }
        }
    }

    states: [
        State {
            name: "library"
            PropertyChanges { target: locationText; text: "Library" }
        },
        State {
            name: "artists"
            PropertyChanges { target: locationText; text: "Library > Artists" }
        },
        State {
            name: "albums"
            PropertyChanges { target: locationText; text: "Library > Artists > Albums" }
        },
        State {
            name: "songs"
            PropertyChanges { target: locationText; text: "Library > Artists > Albums > Songs" }
        }
    ]

    ScrollBar {
         scrollArea: list; width: 15; alwaysVisible: false
         anchors { right: parent.right; top: topMenu.bottom; bottom: parent.bottom}
         anchors.topMargin: 20
         anchors.bottomMargin: 20
     }


}
