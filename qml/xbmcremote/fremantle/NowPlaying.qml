import Qt 4.7

Item {
    id: nowPlayling
    anchors.fill: parent

    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: playlist.currentItem

    state: "nowPlaying"

    NowPlayingView {
        id: nowPlayingView
        anchors.fill: parent
        state: "hidden"
        onOpenPlaylist: {
            console.log("clicked3")
            nowPlayling.state = "playlist"
        }
    }

    AudioPlaylist {
        id: audioPlaylist
        anchors.fill: parent
        model: playlist
        state: "hidden"
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
