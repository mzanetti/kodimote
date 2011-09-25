import Qt 4.7

Row {
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()

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
                player.skipPrevious()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: (player.speed < 1 && player.speed !== 0 ) ? "images/OSDRewindFO.png" : "images/OSDRewindNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.seekBackward()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: (player.speed != 1 || player.state != "playing") ? "images/OSDPlayNF.png" : "images/OSDPauseNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.playPause()
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
                player.stop()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: player.speed > 1 ? "images/OSDForwardFO.png" : "images/OSDForwardNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.seekForward()
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
                player.skipNext()
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
