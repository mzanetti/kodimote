import Qt 4.7
import Xbmc 1.0

BorderImage {
    id: nowPlayingView
    border.top: 15
    border.right: 15
    border.left: 15
    border.bottom: 15
    anchors.margins: 10
    anchors.fill: parent
    source: "images/ContentPanel.png"

    signal openPlaylist

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
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        opacity: 0

        states: [
            State { name: "volumeVisible"
                PropertyChanges {target: volumeBar; anchors.topMargin: 10; opacity: 1}
            }
        ]

        Behavior on anchors.topMargin {
            NumberAnimation{ duration: 200 }
        }
        Behavior on opacity {
            NumberAnimation{ duration: 200 }
        }
    }

    Item {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15
        height: 60

        PlayerControlsTop {
            anchors.fill: parent

            onOpenPlaylist: {
                console.log("clicked2*")
                nowPlayingView.openPlaylist()
            }
        }

    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.bottom: topBar.bottom
        height: 1
        color: "#ffffff";
    }

    function fanart() {
        if(ActivePlaylist.currentFanart.length == 0 || ActivePlaylist.currentFanart == "DefaultAlbumCover.png") {
            return "images/DefaultAlbumCover.png"
        }
        return Xbmc.vfsPath + ActivePlaylist.currentThumbnail
    }

    Flow {
        id: nowPlayingFlow
        anchors.top: volumeBar.bottom
        anchors.bottom: audioPlayerControls.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        spacing: 15

        Item {
            id: imageItem
            width: MainWindow.state == "portrait" ? nowPlayingFlow.width : height
            height: MainWindow.state == "portrait" ? nowPlayingFlow.height - nowPlayingText.height - nowPlayingFlow.spacing : nowPlayingFlow.height

            Image {
                anchors.centerIn: parent
                height: parent.height
                width: height
                source: fanart()
            }
        }


        Item {
            id: nowPlayingText
            width: MainWindow.state == "portrait" ? nowPlayingFlow.width : nowPlayingFlow.width - imageItem.width - nowPlayingFlow.spacing
            height: MainWindow.state == "portrait" ? 40 * 3 + 20: nowPlayingFlow.height

            Item {
                id: line1
                height: 40
                width: nowPlayingText.width
                anchors.bottom: ActivePlayer.type == AudioPlayer.PlayerTypeAudio ? artistText.top : titleText.top

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
                    anchors.rightMargin: trackNumText.width + 20
                    height:  40
                    color: "#0084ff"
                    text: "Track:"
                    font.pixelSize: 22
                    visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio
                }

                Text {
                    id: trackNumText
                    height:  40
                    anchors.right: parent.right
                    color: "white"
                    text: AudioPlaylist.currentTrackNumber + " / " + AudioPlaylist.count
                    font.pixelSize: 22
                    visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio
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
                visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio
            }

            Text {
                id:titleText
                width: nowPlayingText.width
                height:  40
                anchors.bottom: progressBar.top
                anchors.bottomMargin: 5
                color: "white"
                text: ActivePlaylist.currentTitle.length > 0 ? ActivePlaylist.currentTitle : ActivePlaylist.currentLabel
                font.pixelSize: 32
                font.bold: true
                elide: Text.ElideRight
            }

            Rectangle {
                id: progressBar
                width: nowPlayingText.width
                height: 15
                anchors.bottom: nowPlayingText.bottom
                color: "#1d1d1d"
                border.color: "#333333"
                border.width: 2
                radius: height/2
            }

            Rectangle {
                width: height + (progressBar.width - height) * ActivePlayer.percentage / 100
                height: 15
                anchors.bottom: nowPlayingText.bottom
                radius: height/2
                color: "#8fb2c2"
                border.color: "#c7d9e2"
                border.width: 2

                Behavior on width {

                    NumberAnimation {duration: 500; easing.type: Easing.InOutQuad }
                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.bottom: audioPlayerControls.top
        height: 1
        color: "#ffffff";
    }

    AudioPlayerControls {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        height: 60
        id: audioPlayerControls
    }

    states: [
        State {
            name: "hidden"; when: !visible
            PropertyChanges { target: nowPlayingView; opacity: 0}
        },
        State {
            name: "visible"; when: visible
            PropertyChanges { target: nowPlayingView; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { property: "opacity"; duration: 300; easing.type: Easing.InQuint }

        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { property: "opacity"; duration: 300; easing.type: Easing.OutQuint }

        }
    ]

}
