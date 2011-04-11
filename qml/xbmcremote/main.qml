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
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: bottomMenu.top }
        model: itemModel
        preferredHighlightBegin: 0; preferredHighlightEnd: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem; flickDeceleration: 300
        highlightMoveSpeed: 1200
        cacheBuffer: 200
    }

    Rectangle {

        id: bottomMenu
        width: parent.width; height: 60
        anchors.bottom: parent.bottom
        color: "gray"
        gradient: Gradient {
            GradientStop {
                position: 1.00;
                color: "#000000";
            }
            GradientStop {
                position: 0.00;
                color: "#979797";
            }
        }

        Row {
            anchors.centerIn: parent
            spacing: 0

            Repeater {
                model: itemModel.count

                Item {
                    id: menuButton
                    width: bottomMenu.width / itemModel.count;
                    height: bottomMenu.height
                    Rectangle {
                        anchors.fill: parent
                        radius: 3
                        opacity: view.currentIndex == index ? 0.2 : 0
                        color: "#0084ff"

                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: view.currentIndex = index
                    }
                    Text {
                        anchors.centerIn: parent
                        text: indexLabel()
                        color: "white"
                        opacity: 1
                    }
                    function indexLabel() {
                        if(index == 0) {
                            return "Library";
                        }
                        if(index == 1) {
                            return "Playlist";
                        }
                        if(index == 2) {
                            return "Now Playing";
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: tmpModel
        ListElement { label: "bla" }
        ListElement { label: "bla" }
        ListElement { label: "bla" }
    }



    states: [
        State {
            name: "library"
        },
        State {
            name: "audio"
        },
        State {
            name: "video"
        }
    ]
}
