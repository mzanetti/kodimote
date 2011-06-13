import Qt 4.7

Item {
    id: nowPlayling
    anchors.fill: parent

    property alias playlist: audioPlaylist.model

    state: "nowPlaying"

    NowPlayingView {
        id: nowPlayingView
        anchors.fill: parent
        onOpenPlaylist: {
            console.log("clicked3")
            nowPlayling.state = "playlist"
        }
    }

    AudioPlaylist {
        id: audioPlaylist
        anchors.fill: parent
        onClosePlaylist: {
            nowPlayling.state = "nowPlaying"
        }
    }

    states: [
        State {
            name: "nowPlaying"
            PropertyChanges { target: nowPlayingView; state: "visible" }
            PropertyChanges { target: audioPlaylist; state: "hidden" }
        },
        State {
            name: "playlist"
            PropertyChanges { target: nowPlayingView; state: "hidden" }
            PropertyChanges { target: audioPlaylist; state: "visible" }
        }

    ]
}
