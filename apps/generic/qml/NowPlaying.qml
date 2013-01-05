import Qt 4.7
import Xbmc 1.0

FocusScope {
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
        focus: true
        onOpenPlaylist: {
            console.log("clicked3")
            nowPlayling.state = "playlist"
            audioPlaylist.forceActiveFocus();
        }
    }

    AudioPlaylist {
        id: audioPlaylist
        anchors.fill: parent
        model: playlist
        state: "hidden"
        onClosePlaylist: {
            nowPlayling.state = "nowPlaying"
            nowPlayingView.forceActiveFocus();
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
