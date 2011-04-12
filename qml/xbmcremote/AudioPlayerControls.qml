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
            source: AudioPlayer.speed < 1 ? "images/OSDRewindFO.png" : "images/OSDRewindNF.png"
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
            source: (AudioPlayer.speed != 1 || AudioPlayer.state != "playing") ? "images/OSDPlayNF.png" : "images/OSDPauseNF.png"
            anchors.centerIn: parent
        }
        function playButtonSource() {
            if(AudioPlayer.speed != 1) {
                return "images/OSDPlayNF.png"
            }
            if(AudioPlayer.state == "playing") {
                return "images/OSDPauseNF.png"
            }
            return "images/OSDPlayNF.png"
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
            source: AudioPlayer.speed > 1 ? "images/OSDForwardFO.png" : "images/OSDForwardNF.png"
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
