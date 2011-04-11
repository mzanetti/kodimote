import QtQuick 1.0

Rectangle {
    color: "black"
    id: nowPlaying
    state: "none"

    Image {
        anchors.top: topBar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        width: 300; height: 300
        source: Xbmc.vfsPath + AudioPlaylist.currentThumbnail
        onSourceChanged: console.log("ÄÄÄÄ" + source)
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
                PropertyChanges {target: topButtonModel.get(4); imageFile: "images/OSDAudioFO.png"}
            }
        ]

        Behavior on anchors.topMargin {
            NumberAnimation{ duration: 200 }
        }
    }

    ListModel {
        id: topButtonModel
        ListElement { buttonId: 0; imageFile: "images/OSDPresetSettingsNF.png" }
        ListElement { buttonId: 1; imageFile: "images/OSDRepeatNF.png" }
        ListElement { buttonId: 2; imageFile: "images/OSDRandomOffNF.png" }
        ListElement { buttonId: 3; imageFile: "images/OSDPlaylistNF.png" }
        ListElement { buttonId: 4; imageFile: "images/OSDAudioNF.png" }
    }

    Rectangle {
        id: topBar
        color: "black"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 100

        ListView {
            id: topButtonView
            model: topButtonModel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            orientation: ListView.Horizontal
            height: 100
            z: 2
            interactive: false

            delegate: Item {
                id: myDelegate
                height: parent.height
                width: topBar.width / topButtonModel.count
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
    //                        AudioPlayer.skipPrevious()
                            break;
                        case 1:
    //                        AudioPlayer.seekBackward()
                            break;
                        case 2:
    //                        AudioPlayer.playPause()
                            break;
                        case 3:
    //                        AudioPlayer.stop()
                            break;
                        case 4:
                            if(volumeBar.state == "volumeVisible") {
                                console.log("WTF")
                                volumeBar.state = "none"
                            } else {
                                console.log("setting state to volbar")
                                volumeBar.state = "volumeVisible"
                            }
                            console.log("fdsfsdafsdadf")

                            break;
                        }
                    }
                }
            }
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


    Text {
        id: nowPlayingText
        anchors {bottom: artistText.top; left: parent.left }
        anchors.leftMargin: 15
        height:  40
        color: "#0084ff"
        text: "Now playing:"
        font.pixelSize: 22
    }

    Text {
        id: trackNumLabel
        anchors {bottom: artistText.top; right: trackNumText.right}
        anchors.rightMargin: trackNumText.width + 15
        height:  40
        color: "#0084ff"
        text: "Track:"
        font.pixelSize: 22
    }
    Text {
        id: trackNumText
        anchors {bottom: artistText.top; right: parent.right}
        anchors.rightMargin: 15
        height:  40
        color: "white"
        text: AudioPlaylist.currentTrackNumber + " / " + AudioPlaylist.count
        font.pixelSize: 22
    }

    Text {
        id: artistText
        anchors {bottom: titleText.top; left: parent.left; right: parent.right }
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        height:  40
        color: "white"
        text: AudioPlaylist.currentArtist + " - " + AudioPlaylist.currentAlbum
        font.pixelSize: 22
    }

    Text {
        id:titleText
        anchors {bottom: audioPlayerControls.top; left: parent.left; right: parent.right }
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        height:  40
        color: "white"
        text: AudioPlaylist.currentTitle
        font.pixelSize: 32
        font.bold: true
    }

    AudioPlayerControls {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 100
        id: audioPlayerControls
    }
}
