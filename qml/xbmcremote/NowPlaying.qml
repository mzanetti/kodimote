import QtQuick 1.0

Rectangle {
    color: "black"
    id: nowPlaying
    state: AudioPlayer.state

    Connections {
        target: MainWindow
        onOrientationChanged: console.log("orientationchanged")
    }

    VolumeBar {
        id: volumeBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.topMargin: -height

        states: [
            State { name: "volumeVisible"
                PropertyChanges {target: volumeBar; anchors.topMargin: 0;}
            }
        ]

        Behavior on anchors.topMargin {
            NumberAnimation{ duration: 200 }
        }
    }

    Rectangle {
        id: topBar
        color: "black"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 100

        PlayerControlsTop {
            anchors.fill: parent
        }

    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottom: topBar.bottom
        height: 1
        color: "#ffffff";
    }

    function fanart() {
        if(AudioPlaylist.currentFanart.length == 0 || AudioPlaylist.currentFanart == "DefaultAlbumCover.png") {
            return "images/DefaultAlbumCover.png"
        }
        return Xbmc.vfsPath + AudioPlaylist.currentThumbnail
    }

    Flow {
        id: nowPlayingFlow
        anchors.top: volumeBar.bottom
        anchors.bottom: audioPlayerControls.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 15
        spacing: 15

//        Rectangle { height: 30; width: nowPlayingList.width; color: "blue" }

        Item {
            id: imageItem
            width: MainWindow.state == "portrait" ? nowPlayingFlow.width : height
            height: MainWindow.state == "portrait" ? nowPlayingFlow.height - nowPlayingText.height - nowPlayingFlow.spacing : nowPlayingFlow.height

            Image {
                anchors.centerIn: parent
                height: parent.height
                width: height
                source: fanart()
                onSourceChanged: console.log("ÄÄÄÄ" + source)
            }
        }


        Item {
            id: nowPlayingText
            width: MainWindow.state == "portrait" ? nowPlayingFlow.width : nowPlayingFlow.width - imageItem.width - nowPlayingFlow.spacing
            height: MainWindow.state == "portrait" ? 40 * 3 : nowPlayingFlow.height

            Item {
                id: line1
                height: 40
                width: nowPlayingText.width
                anchors.bottom: artistText.top

                Text {
                    id: nowPlayingTextLabel
                    height:  40
                    anchors.fill: line1
                    color: "#0084ff"
                    text: "Now playing:"
                    font.pixelSize: 22
                }

                Text {
                    id: trackNumLabel
                    anchors.right: trackNumText.right
                        anchors.rightMargin: trackNumText.width + 15
                    height:  40
                    color: "#0084ff"
                    text: "Track:"
                    font.pixelSize: 22
                }

                Text {
                    id: trackNumText
                    height:  40
                    anchors.right: parent.right
                    color: "white"
                    text: AudioPlaylist.currentTrackNumber + " / " + AudioPlaylist.count
                    font.pixelSize: 22
                }
            }


            Text {
                id: artistText
                height:  40
                width: nowPlayingText.width
                anchors.bottom: titleText.top
                color: "white"
                text: AudioPlaylist.currentArtist + " - " + AudioPlaylist.currentAlbum
                font.pixelSize: 22
                elide: Text.ElideRight
            }

            Text {
                id:titleText
                width: nowPlayingText.width
                height:  40
                anchors.bottom: nowPlayingText.bottom
                color: "white"
                text: AudioPlaylist.currentTitle
                font.pixelSize: 32
                font.bold: true
                elide: Text.ElideRight
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottom: audioPlayerControls.top
        height: 1
        color: "#ffffff";
    }

    AudioPlayerControls {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 100
        id: audioPlayerControls
    }

}
