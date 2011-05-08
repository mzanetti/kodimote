import QtQuick 1.0

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
            PropertyChanges { target: nowPlayingView; opacity: 1 }
            PropertyChanges { target: audioPlaylist; state: "hidden" }
        },
        State {
            name: "playlist"
            PropertyChanges { target: nowPlayingView; opacity: 0 }
            PropertyChanges { target: audioPlaylist; state: "visible" }
        }

    ]
}
