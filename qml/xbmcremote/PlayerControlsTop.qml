import QtQuick 1.0

Row {
    anchors.fill: parent
    property bool volumeVisible: false

    Item {
        height: parent.height
        width: parent.width / 5

        Image {
            source: "images/OSDPresetSettingsNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 5

        Image {
            source: "images/OSDRepeatNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 5

        Image {
            source: AudioPlaylist.shuffle ? "images/OSDRandomOffFO.png" : "images/OSDRandomOffNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                AudioPlaylist.shuffle = !AudioPlaylist.shuffle
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 5

        Image {
            source: "images/OSDPlaylistNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 5

        Image {
            source: volumeBar.state == "volumeVisible" ? "images/OSDAudioFO.png" : "images/OSDAudioNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(volumeBar.state == "volumeVisible") {
                    console.log("WTF")
                    volumeBar.state = "none"
                } else {
                    console.log("setting state to volbar")
                    volumeBar.state = "volumeVisible"
                }
            }
        }
    }
}

