import QtQuick 1.0

Row {

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDPrevTrackNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.skipPrevious()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDRewindNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.seekBackward()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: AudioPlayer.state == "playing" ? "images/OSDPauseNF.png" : "images/OSDPlayNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.playPause()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDStopNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.stop()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDForwardNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.seekForward()
            }
        }
    }
    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDNextTrackNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlayer.skipNext()
            }
        }
    }
    states: [
        State {
            name: "stopped"
        },
        State {
            name: "playing"
        },
        State {
            name: "paused"
        }

    ]
}
