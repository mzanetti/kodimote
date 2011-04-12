import QtQuick 1.0

Rectangle {
    color: "black"
    id: nowPlaying
    state: AudioPlayer.state

    Image {
        anchors.top: volumeBar.bottom
        anchors.bottom: nowPlayingText.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        width: height
        source: fanart()
        onSourceChanged: console.log("ÄÄÄÄ" + source)
    }
    function fanart() {
        if(AudioPlaylist.currentFanart.length == 0 || AudioPlaylist.currentFanart == "DefaultAlbumCover.png") {
            return "images/DefaultAlbumCover.png"
        }
        return Xbmc.vfsPath + AudioPlaylist.currentThumbnail
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
        elide: Text.ElideRight
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
        elide: Text.ElideRight
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
