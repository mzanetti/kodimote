import QtQuick 1.0

Rectangle {
    width: 480
    height: 800
    color: "black"

    state: Xbmc.state

    onStateChanged: {
        console.log("*****" + state)
    }

    VisualItemModel {
        id: itemModel

        Rectangle {
            property string label: "Library"
            width: view.width; height: view.height
            LibraryView {
                id: libraryView
                visible: true
            }

            Component.onDestruction: print("destroyed 1")
        }
        Rectangle {
            property string label: "Playlist"
            width: view.width; height: view.height
            AudioPlaylist {
                id: audioPlaylist
                anchors.fill: parent
                model: AudioPlaylist
            }

            Component.onDestruction: print("destroyed 3")
        }
        Rectangle {
            property string label: "Now Playing"
            width: view.width; height: view.height
            NowPlaying {
                anchors.fill: parent
            }

            Component.onDestruction: print("destroyed 2")
        }
    }

    ListView {
        id: view
        anchors.fill: parent
        model: itemModel
        preferredHighlightBegin: 0; preferredHighlightEnd: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem; flickDeceleration: 300
        highlightMoveSpeed: 1200
        cacheBuffer: 200
    }

}
