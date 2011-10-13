import Qt 4.7

FocusScope {
    id: playlist
    anchors.fill: parent
    property QtObject player: xbmc.activePlayer
    property alias model: list.model
    property alias view: list

    signal closePlaylist

    Keys.onPressed: {
        switch(event.key) {
        case Qt.Key_Down:
            audioPlaylist.view.incrementCurrentIndex();
            break;
        case Qt.Key_Up:
            audioPlaylist.view.decrementCurrentIndex();
            break;
        case Qt.Key_Enter:
        case Qt.Key_Return:
            player.playItem(audioPlaylist.view.currentIndex);
            break;
        case Qt.Key_Space:
            contextMenu.forceActiveFocus();
            break;
        case Qt.Key_Escape:
        case Qt.Key_Backspace:
            playlist.closePlaylist();
            break;
        }
    }

    Image {
        id: header
        anchors.left: parent.left
        anchors.top: parent.top
        height: 60
        source: "images/header.png"

        Text {
            anchors.fill: parent
            anchors.margins: 10
            text: "Now Playing - Playlist"
            color: "white"
            font.pixelSize: 16
            verticalAlignment: Text.AlignVCenter

        }
    }

    Image {
        id: nowPlayingButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 5
        anchors.rightMargin: 10
        width: 64
        height: width
        source: "images/NowPlayingIcon.png"

        Behavior on anchors.rightMargin {
            NumberAnimation { duration:  300; easing.type: Easing.OutQuad}
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                playlist.closePlaylist()
            }
        }
    }

    BorderImage {
        id: background
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: header.bottom }
        source: "images/ContentPanel.png"

        Behavior on scale {
            NumberAnimation { duration: 300; easing.type: Easing.OutBack }
        }

        ListView {
            id: list
            anchors.fill: parent
            anchors.margins: 25
            clip: true
            model: playlist.model
            currentIndex: 0

            delegate: Item {
                width: list.width
                height: 64
                //            Rectangle {
                //                width: parent.width
                //                height: 1
                //                anchors.top: parent.bottom
                //                color: "white"
                //                opacity: 0.5

                //            }
                Image {
                    anchors.fill: parent
                    source: itemArea.pressed || index == list.currentIndex ? "images/MenuItemFO.png" : "images/MenuItemNF.png"
                }
                Image {
                    anchors {top: parent.top; right: parent.right; bottom: parent.bottom }
                    anchors.topMargin: 10
                    anchors.bottomMargin: 10
                    source: "images/MediaItemDetailBG.png"
                    visible: itemArea.pressed || index == list.currentIndex
                }

                Text {
                    color: list.model.currentTrackNumber - 1 == index ? "yellow" : "white"
                    text: title
                    font.pixelSize: 28
                    anchors {left: parent.left; top:  parent.top; bottom: parent.bottom; right: durationText.left }
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                Text {
                    id: durationText
                    color: list.model.currentTrackNumber - 1 == index ? "yellow" : "white"
                    text: duration
                    font.pixelSize: 28
                    anchors { top: parent.top; right: parent.right; bottom: parent.bottom }
                    anchors.rightMargin: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    id: itemArea
                    enabled: contextMenu.state != "visible"

                    onPressed: {
                        list.currentIndex = index
                    }

                    onPressAndHold: {
                        contextMenu.open();
                    }

                    onClicked: {
                        player.playItem(index)
                    }
                }
            }
        }

        ScrollBar {
            scrollArea: list; width: 15; alwaysVisible: false
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom}
            anchors.rightMargin: 10
            anchors.topMargin: 20
            anchors.bottomMargin: 20
        }

    }


    ContextMenu {
        id: contextMenu

        model: ListModel {
            ListElement { entryId: 0; menuEntry: "Play"}
            ListElement { entryId: 1; menuEntry: "Remove from playlist"}
            ListElement { entryId: 2; menuEntry: "Clear playlist"}
        }

        onAccepted: {
            switch(index) {
            case 0:
                list.model.playItem(list.currentIndex);
                break;
            case 1:
                list.model.removeItem(list.currentIndex);
                break;
            case 2:
                list.model.clear();
                break;
            }
            print("blablalb")
            contextMenu.focus = false;
            playlist.forceActiveFocus();
        }

        onRejected: {
            contextMenu.focus = false;
            playlist.forceActiveFocus();
        }
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges { target: background; opacity: 0 }
            PropertyChanges { target: header; opacity: 0; anchors.leftMargin: -header.width }
            PropertyChanges { target: nowPlayingButton; opacity: 0; anchors.rightMargin: -nowPlayingButton.width }
        },
        State {
            name: "visible"
            PropertyChanges { target: background; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { target: background; property: "opacity"; duration: 300; easing.type: Easing.InQuint }
            NumberAnimation { target: header; property: "anchors.leftMargin"; duration:  300; easing.type: Easing.OutQuad}
            NumberAnimation { target: nowPlayingButton; property: "anchors.rightMargin"; duration:  300; easing.type: Easing.OutQuad}
        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { property: "opacity"; duration: 300; easing.type: Easing.OutQuint }
            NumberAnimation { target: header; property: "anchors.leftMargin"; duration:  200; easing.type: Easing.InQuad}
            NumberAnimation { target: header; property: "opacity"; duration: 300; easing.type: Easing.OutQuad}
            NumberAnimation { target: nowPlayingButton; property: "anchors.rightMargin"; duration: 200; easing.type: Easing.InQuad}
            NumberAnimation { target: nowPlayingButton; property: "opacity"; duration: 300; easing.type: Easing.OutQuad}
        }
    ]
}

