import Qt 4.7
import Xbmc 1.0

Row {
    id: playerControlsTop
    anchors.fill: parent
    property bool volumeVisible: false

    signal openPlaylist

//    Item {
//        height: parent.height
//        width: parent.width / 5

//        Image {
//            source: "images/OSDPresetSettingsNF.png"
//            anchors.centerIn: parent
//        }

//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//            }
//        }
//    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            anchors.centerIn: parent
            source: xbmc.activePlayer.playlist().repeat == Playlist.RepeatNone ? "images/OSDRepeatNF.png" : (xbmc.activePlayer.playlist().repeat == Playlist.RepeatOne ? "images/OSDRepeatOneFO.png" : "images/OSDRepeatAllFO.png")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(xbmc.activePlayer.playlist().repeat == Playlist.RepeatNone) {
                    xbmc.activePlayer.playlist().repeat = Playlist.RepeatOne;
                } else if(xbmc.activePlayer.playlist().repeat == Playlist.RepeatOne) {
                    xbmc.activePlayer.playlist().repeat = Playlist.RepeatAll;
                } else {
                    xbmc.activePlayer.playlist().repeat = Playlist.RepeatNone;
                }
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: xbmc.activePlayer.playlist().shuffle ? "images/OSDRandomOffFO.png" : "images/OSDRandomOffNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                xbmc.activePlayer.playlist().shuffle = ! xbmc.activePlayer.playlist().shuffle
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: "images/OSDPlaylistNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("clicked1")
                playerControlsTop.openPlaylist()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: volumeBar.state == "volumeVisible" ? "images/OSDAudioFO.png" : "images/OSDAudioNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(volumeBar.state == "volumeVisible") {
                    volumeBar.state = "none"
                } else {
                    volumeBar.state = "volumeVisible"
                }
            }
        }
    }
}

