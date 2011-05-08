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
                ActivePlayer.skipPrevious()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: ActivePlayer.speed < 1 ? "images/OSDRewindFO.png" : "images/OSDRewindNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ActivePlayer.seekBackward()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: (ActivePlayer.speed != 1 || ActivePlayer.state != "playing") ? "images/OSDPlayNF.png" : "images/OSDPauseNF.png"
            anchors.centerIn: parent
        }
        function playButtonSource() {
            if(ActivePlayer.speed != 1) {
                return "images/OSDPlayNF.png"
            }
            if(ActivePlayer.state == "playing") {
                return "images/OSDPauseNF.png"
            }
            return "images/OSDPlayNF.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ActivePlayer.playPause()
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
                ActivePlayer.stop()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: ActivePlayer.speed > 1 ? "images/OSDForwardFO.png" : "images/OSDForwardNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ActivePlayer.seekForward()
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
                ActivePlayer.skipNext()
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
