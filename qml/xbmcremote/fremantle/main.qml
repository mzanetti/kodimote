import Qt 4.7

Rectangle {
    id: screen
//    anchors.fill: parent
    color: "black"
//    width: screen.displayWidth
//    height: screen.displayHeight
//    width: 500
//    height: 500

    property string orientation:  width > height ? "landscape" : "portrait"

//    state: xbmc.state

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image  {
        id: backgroundImageMusic
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "backgrounds/music.jpg"
        opacity:  xbmcBrowser.mediaState == "audio" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

    Image {
        id: backgroundImageVideos
        source: "backgrounds/videos.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        opacity:  xbmcBrowser.mediaState == "video" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

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
            id: xbmcBrowser
            width: view.width; height: view.height

            property string mediaState: "audio"
            property string viewState: "library"

            LibraryView {
                id: musicBrowser
                library: xbmc.shares("music")
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: musicLibraryView
                library: xbmc.audioLibrary()
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: videoBrowser
                library: xbmc.shares("video")
                visible: false
                onGoBack: homeMenu.state = "open"
            }
            LibraryView {
                id: videoLibraryView
                library: xbmc.videoLibrary()
                visible: false
                onGoBack: homeMenu.state = "open"
            }

            states: [
                State { name: "none"; when: homeMenu.state == "open"
                    PropertyChanges {target:  musicBrowser; visible: false }
                    PropertyChanges {target:  musicLibraryView; visible: false }
                    PropertyChanges {target:  videoBrowser; visible: false }
                    PropertyChanges {target:  videoLibraryView; visible: false }
                },
                State { name: "audiofiles"; when: xbmcBrowser.mediaState == "audio" && xbmcBrowser.viewState == "files"
                    PropertyChanges {target:  musicBrowser; visible: true }
                },
                State { name: "audiolibrary"; when: xbmcBrowser.mediaState == "audio" && xbmcBrowser.viewState == "library"
                    PropertyChanges {target:  musicLibraryView; visible: true }
                },
                State { name: "videofiles"; when: xbmcBrowser.mediaState == "video" && xbmcBrowser.viewState == "files"
                    PropertyChanges {target:  videoBrowser; visible: true }
                },
                State { name: "videolibrary"; when: xbmcBrowser.mediaState == "video" && xbmcBrowser.viewState == "library"
                    PropertyChanges {target:  videoLibraryView; visible: true }
                }

            ]

        }

        Item {
            property string label: "Now Playing"
            width: view.width; height: view.height
            NowPlaying {
                anchors.fill: parent
                playlist: xbmc.activePlayer.playlist()
            }

        }

        Item {
            width: view.width; height: view.height
            Keypad {}
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


    MouseArea {
        id: cancelArea
        anchors.fill: screen
        enabled: false
        onClicked: {
            homeMenu.state = "closed"
            homeMenu.subMenuState = "closed"
        }
        states: State {
            name: "homeMenuOpen"; when: homeMenu.state == "open" || homeMenu.subMenuState == "open"
            PropertyChanges { target: cancelArea; enabled: true }
        }
    }
    HomeMenu {
        id: homeMenu

    }

    states: [
    State {
            name: "notConnected"; when: !xbmc.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0.8; scale: 1 }
            PropertyChanges { target: view; opacity: 0; }
        },
        State {
            name: "connected"; when: xbmc.connected
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
