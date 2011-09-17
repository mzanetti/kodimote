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
        opacity:  xbmcBrowser.mediaState == "pictures" ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
    }

    FocusScope {
        anchors.fill: parent
        focus: true
        property string inputText
        Timer {
            id: inputTimer
            interval: 1500
            repeat: false
            triggeredOnStart: false

            onTriggered: parent.inputText = ""
        }

        Keys.onPressed: {
            inputTimer.stop();
            inputText += event.text;
            itemModel.currentView.view.positionViewAtIndex(itemModel.currentView.library.findItem(inputText), ListView.Beginning)
            inputTimer.start();
        }


    }

    VisualItemModel {
        id: itemModel

        property QtObject currentView
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
            LibraryView {
                id: pictureBrowser
                library: xbmc.shares("pictures")
                visible: false
                onGoBack: homeMenu.state = "open"
            }

            states: [
                State { name: "none"; when: homeMenu.state == "open"
                    PropertyChanges {target:  musicBrowser; visible: false }
                    PropertyChanges {target:  musicLibraryView; visible: false }
                    PropertyChanges {target:  videoBrowser; visible: false }
                    PropertyChanges {target:  videoLibraryView; visible: false }
                    PropertyChanges {target: itemModel; currentView: null }
                },
                State { name: "audiofiles"; when: xbmcBrowser.mediaState == "audio" && xbmcBrowser.viewState == "files"
                    PropertyChanges {target:  musicBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: musicBrowser }
                },
                State { name: "audiolibrary"; when: xbmcBrowser.mediaState == "audio" && xbmcBrowser.viewState == "library"
                    PropertyChanges {target:  musicLibraryView; visible: true }
                    PropertyChanges {target: itemModel; currentView: musicLibraryView }
                },
                State { name: "videofiles"; when: xbmcBrowser.mediaState == "video" && xbmcBrowser.viewState == "files"
                    PropertyChanges {target:  videoBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: videoBrowser }
                },
                State { name: "videolibrary"; when: xbmcBrowser.mediaState == "video" && xbmcBrowser.viewState == "library"
                    PropertyChanges {target:  videoLibraryView; visible: true }
                    PropertyChanges {target: itemModel; currentView: videoLibraryView }
                },
                State { name: "pictures"; when: xbmcBrowser.mediaState == "pictures" && xbmcBrowser.viewState == "pictures"
                    PropertyChanges {target:  pictureBrowser; visible: true }
                    PropertyChanges {target: itemModel; currentView: pictureBrowser }
                }

            ]

        }

        Item {
            property string label: "Now Playing"
            width: view.width
            height: view.height
            NowPlaying {
                anchors.fill: parent
                playlist: xbmc.activePlayer.playlist()
                state: "nowPlaying"
                visible: xbmcBrowser.mediaState != "pictures"
            }

            PictureControls {
                visible: xbmcBrowser.mediaState == "pictures"
                width: view.width
                height: view.height
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
        height: 250
        width: 450
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
            anchors.margins: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: xbmc.connectionError
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
        state: "open"
    }

    states: [
    State {
            name: "notConnected"; when: !xbmc.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0.8; scale: 1 }
            PropertyChanges { target: view; opacity: 0; }
            PropertyChanges { target: homeMenu; opacity: 0 }
        },
        State {
            name: "connected"; when: xbmc.connected
            PropertyChanges { target: notConnectedPopup; opacity: 0; scale: 0.5 }
            PropertyChanges { target: view; opacity: 1; }

            PropertyChanges {
                target: backgroundImageMusic
                x: 0
                y: 0
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
            }
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
