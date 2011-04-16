import QtQuick 1.0

Image {
    width: 480
    height: 800
    source: "backgrounds/music.jpg"

    state: Xbmc.state

    VisualItemModel {
        id: itemModel

//        Item {
//            property string label: "Library"
//            width: view.width; height: view.height
//            LibraryView {
//                id: libraryView
//                visible: true
//            }

//            Component.onDestruction: print("destroyed 1")
//        }
        Item {
            id: musicBrowser
            width: view.width; height: view.height
            Browser {
                id: fileBrowser
                visible: true
                onGoBack: selectorPopup.state = "open"
            }
            LibraryView {
                id: libraryView
                visible: false
                onGoBack: selectorPopup.state = "open"
            }

            states: [
                State { name: "files"
                    PropertyChanges {target:  fileBrowser; visible: true }
                    PropertyChanges {target:  libraryView; visible: false }
                },
                State { name: "library"
                    PropertyChanges {target:  fileBrowser; visible: false }
                    PropertyChanges {target:  libraryView; visible: true }
                }

            ]

        }

        Item {
            property string label: "Playlist"
            width: view.width; height: view.height
            AudioPlaylist {
                id: audioPlaylist
                anchors.fill: parent
                model: AudioPlaylist
            }

        }
        Item {
            property string label: "Now Playing"
            width: view.width; height: view.height
            NowPlaying {
                anchors.fill: parent
            }

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

    Rectangle {
        id: notConnectedPopup
        anchors.centerIn: parent
        height: 200
        width: 400
        color: "black"
        border.color: "white"
        radius: 10
        opacity: 0
        scale: 0.5

        Image {
            anchors.fill: parent
            source: "images/GlassOverlay.png"
        }

        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Could not connect to Xbmc. Please check your network connection and settings."
            wrapMode: Text.WordWrap
            color: "white"
            font.pixelSize: 28
        }
    }

    Rectangle {
        id: selectorPopup
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.left
        width: 300
        color: "black"
        state: "closed"

        ListModel {
            id: musicSelectorModel
            ListElement { label: "Files"}
            ListElement { label: "Library"}
        }
        ListView {
            anchors.fill: parent
            model: musicSelectorModel
            delegate: Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 55

                Text {
                    text: label
                    color: "white"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(index == 0) {
                            musicBrowser.state = "files"
                        } else {
                            musicBrowser.state = "library"
                        }
                        selectorPopup.state = "closed"
                    }
                }
            }
        }

        states:  [
            State { name: "open"
                PropertyChanges { target: selectorPopup; anchors.rightMargin: -selectorPopup.width }
            }
        ]
    }

    states: [
    State {
            name: "notConnected"; when: !Xbmc.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0.8; scale: 1 }
            PropertyChanges { target: view; opacity: 0; }
        },
        State {
            name: "connected"; when: Xbmc.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0; scale: 0.5 }
            PropertyChanges { target: view; opacity: 1; }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "notConnected"
            NumberAnimation {properties: "opacity"; duration:  300; easing.type: Easing.InCirc}
            NumberAnimation {properties: "scale"; duration:  500; easing.type: Easing.OutBack}
        },
        Transition {
            from: "notConnected"
            to: "*"
            NumberAnimation {properties: "opacity"; duration:  200; easing.type: Easing.OutCirc}
            NumberAnimation {properties: "scale"; duration:  300; easing.type: Easing.Linear}
        }
    ]


}
