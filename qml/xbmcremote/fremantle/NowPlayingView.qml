import Qt 4.7

FocusScope {
    id: nowPlayingView
    anchors.fill: parent

    BorderImage {
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15
        anchors.margins: 10
        anchors.fill: parent
        source: "images/ContentPanel.png"
    }

    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: player.currentItem

    Keys.onPressed: {
        if(event.modifiers === Qt.ShiftModifier) {
            switch(event.key) {
            case Qt.Key_Left:
                xbmc.activePlayer.seek();
                break;
            case Qt.Key_Right:
                xbmc.activePlayer.seek();
                break;
            }
        } else if(event.modifiers === Qt.NoModifier) {
            switch(event.key) {
            case Qt.Key_R:
                if(xbmc.activePlayer.repeat === Player.RepeatNone) {
                    xbmc.activePlayer.repeat = Player.RepeatOne;
                } else if(xbmc.activePlayer.repeat === Player.RepeatOne) {
                    xbmc.activePlayer.repeat = Player.RepeatAll;
                } else {
                    xbmc.activePlayer.repeat = Player.RepeatNone;
                }
                break;
            case Qt.Key_S:
                xbmc.activePlayer.shuffle = ! xbmc.activePlayer.shuffle
                break;
            case Qt.Key_P:
                openPlaylist();
                break;
            case Qt.Key_Left:
                xbmc.activePlayer.skipPrevious();
                break;
            case Qt.Key_Right:
                xbmc.activePlayer.skipNext();
                break;
            case Qt.Key_Up:
                setVolume(xbmc.volume + 5);
                break;
            case Qt.Key_Down:
                setVolume(xbmc.volume-5);
                break;
            case Qt.Key_Space:
                xbmc.activePlayer.playPause();
                break;
            case Qt.Key_Escape:
                xbmc.activePlayer.stop();
                break;
            case Qt.Key_I:
                itemDetails.forceActiveFocus();
                break;
            }
        }
    }

    function setVolume(value) {
        volumeBar.state = "volumeVisible";
        xbmc.setVolume(value);
        volumeTimer.restart();
    }
    Timer {
        id: volumeTimer
        interval: 3000
        repeat: false
        running: false
        onTriggered: {
            volumeBar.state = "";
        }
    }

    signal openPlaylist()

    VolumeBar {
        id: volumeBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.topMargin: -height
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        opacity: 0

        states: [
            State { name: "volumeVisible"
                PropertyChanges {target: volumeBar; anchors.topMargin: 10; opacity: 1}
            }
        ]

        Behavior on anchors.topMargin {
            NumberAnimation{ duration: 200 }
        }
        Behavior on opacity {
            NumberAnimation{ duration: 200 }
        }
    }

    Item {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 25
        height: 60

        PlayerControlsTop {
            anchors.fill: parent

            onPlaylistButtonClicked: {
                console.log("clicked2*")
                nowPlayingView.openPlaylist();
            }
        }

    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        anchors.bottom: topBar.bottom
        height: 1
        color: "#ffffff";
    }

    Grid {
        id: nowPlayingFlow
        columns: screen.orientation == "portrait" ? 1 : 2
        anchors.top: volumeBar.bottom
        anchors.bottom: audioPlayerControls.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        anchors.topMargin: 15
        anchors.bottomMargin: 15
        spacing: 15

        Image {
            id: imageItem
            height: screen.orientation == "portrait" ? nowPlayingFlow.height - 40 * 3 - 20 - 15: nowPlayingFlow.height
            width: screen.orientation == "portrait" ? nowPlayingFlow.width : height
            source: xbmc.state == "audio" ? "images/DefaultAlbumCover.png" : "images/DefaultVideoCover.png"
            Image {
                anchors.centerIn: parent
                height: Math.min(parent.height, parent.width)
                width: Math.min(parent.height, parent.width)
                source: xbmc.vfsPath + currentItem.thumbnail
                fillMode: Image.PreserveAspectFit
            }
        }


        Item {
            id: nowPlayingText
            width: screen.orientation == "portrait" ? nowPlayingFlow.width : nowPlayingFlow.width - imageItem.width - nowPlayingFlow.anchors.rightMargin
            height: 40 * 3 + 20

            anchors.bottom: nowPlayingFlow.bottom

            Item {
                id: line1
                height: 40
                width: nowPlayingText.width
//                anchors.bottom: xbmc.activePlayer.playlist().type == AudioPlayer.PlayerTypeAudio ? artistText.top : titleText.top

                Text {
                    id: nowPlayingTextLabel
                    height:  40
                    anchors.fill: line1
                    color: "#0084ff"
                    text: qsTr("Now playing:")
                    font.pixelSize: 22
                }

                Text {
                    id: trackNumLabel
                    anchors.right: trackNumText.right
                    anchors.rightMargin: trackNumText.width + 20
                    height:  40
                    color: "#0084ff"
                    text: qsTr("Track:")
                    font.pixelSize: 22
//                    visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio
                }

                Text {
                    id: trackNumText
                    height:  40
                    anchors.right: parent.right
                    color: "white"
                    text: playlist.currentTrackNumber + " / " + playlist.count
                    font.pixelSize: 22
//                    visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio
                }
            }


            Row {
                id: artistText
                height:  40
                width: nowPlayingText.width
                anchors.bottom: titleText.top
                Text {
//                    anchors {left: parent.left; top: parent.top; bottom: parent.bottom; right: ratingStars.left }
                    width: parent.width - ratingStars.width
                    color: "white"
                    text: (xbmc.state == "audio" ? currentItem.artist : (currentItem.type === "episode" ? currentItem.tvShow : "Year: " + currentItem.year)) + (xbmc.state == "audio" ? " - " + currentItem.album : (currentItem.type === "episode" ? " - Season: " + currentItem.season : ""))
                    font.pixelSize: 22
                    elide: Text.ElideRight
    //                visible: ActivePlayer.type == AudioPlayer.PlayerTypeAudio

                }
                Image {
                    id: ratingStars
                    source: "images/rating" + Math.floor(currentItem.rating / 2) + ".png"
                    visible: currentItem.type === "movie"
                }
            }

            Rectangle {
                id: infoButton
                height: 36
                width: height
                radius: height / 2
                border.width: 3
                border.color: "white"
                color: "black"
                opacity: .3
                anchors.right: parent.right
                anchors.top: line1.bottom
            }
            Text {
                anchors.centerIn: infoButton
                font.pixelSize: 20
                text: "i"
                color: "white"
            }
            MouseArea {
                anchors.fill: infoButton
                onClicked: itemDetails.open();
            }

            Text {
                id:titleText
                width: nowPlayingText.width
                height:  40
                anchors.bottom: progressBar.top
                anchors.bottomMargin: 5
                color: "white"
                text: currentItem.title.length > 0 ? currentItem.title : currentItem.label
                font.pixelSize: 32
                font.bold: true
                elide: Text.ElideRight
            }

            Text {
                id: runtimeText
                anchors.bottom: nowPlayingText.bottom
                anchors.right: nowPlayingText.right
                color: "white"
                font.pixelSize: progressBar.height - 2
                text: currentItem.durationString
            }

            Rectangle {
                id: progressBar
                width: nowPlayingText.width - runtimeText.width - 5
                height: 15
                anchors.bottom: nowPlayingText.bottom
                color: "#1d1d1d"
                border.color: "#333333"
                border.width: 2
                radius: height/2
            }

            Rectangle {
                width: height + (progressBar.width - height) * xbmc.activePlayer.percentage / 100
                height: 15
                anchors.bottom: nowPlayingText.bottom
                radius: height/2
                color: "#8fb2c2"
                border.color: "#c7d9e2"
                border.width: 2

                Behavior on width {
                    NumberAnimation {duration: 500; easing.type: Easing.Linear }
                }
            }

            Rectangle {
                id: seekTimeTag
                color: "black"
                visible: seekSlider.opacity > 0.5
                anchors.bottom: seekSlider.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: seekSlider.horizontalCenter
                height: childrenRect.height + 10
                width: childrenRect.width + 10
                radius: 5

                Text {
                    id: seekTimeTagText
                    anchors.centerIn: parent
                    color: "white"
                    text: "00:00:00"
                }
            }

            Rectangle {
                id: seekSlider
                color: "#c7d9e2"
                border.color: "#c7d9e2"
                border.width: 2
                height: progressBar.height * 2
                width: height
                radius: height / 2
                anchors.verticalCenter: progressBar.verticalCenter
                x: dragging ? x : (progressBar.width - height) * xbmc.activePlayer.percentage / 100
                opacity: dragging ? .8 : .001
                property bool dragging: false

                MouseArea {
                    anchors.fill: seekSlider
                    drag.target: parent; drag.axis: Drag.XAxis
                    drag.minimumX: 2; drag.maximumX: progressBar.width - seekSlider.width - 2
                    onPressed: seekSlider.dragging = true
                    onReleased: {
                        player.seek((seekSlider.x) * 100 / (progressBar.width - seekSlider.width))
                        seekSlider.dragging = false
                        print("should seek to" + (seekSlider.x) * 100 / (progressBar.width - seekSlider.width))
                    }
                    onMouseXChanged: {
                        onMouseXChanged: {
                            var ct = (seekTimeTag.x + seekTimeTag.width / 2) * currentItem.durationInSecs / progressBar.width
                            print("ct " + ct);
                            var hours = Math.floor(ct / 60 / 60);
                            var minutes = Math.floor(ct / 60) % 60;
                            if(minutes < 10) minutes = "0" + minutes;
                            var seconds = Math.floor(ct) % 60;
                            if(seconds < 10) seconds = "0" + seconds;
                            if(currentItem.durationInSecs < 60 * 60) {
                                seekTimeTagText.text = minutes + ":" + seconds;
                            } else {
                                seekTimeTagText.text = hours + ":" + minutes + ":" + seconds;
                            }
                        }
                    }

                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 25
        anchors.rightMargin: 25
        anchors.bottom: audioPlayerControls.top
        height: 1
        color: "#ffffff";
    }

    AudioPlayerControls {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 25
        height: 60
        id: audioPlayerControls
    }

    ItemDetails {
        id: itemDetails
        anchors.fill: parent
        item: currentItem
        onClosed: nowPlayingView.forceActiveFocus();
    }

    states: [
        State {
            name: "hidden"; when: !visible
            PropertyChanges { target: nowPlayingView; opacity: 0}
        },
        State {
            name: "visible"; when: visible
            PropertyChanges { target: nowPlayingView; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { property: "opacity"; duration: 300; easing.type: Easing.InQuint }

        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { property: "opacity"; duration: 300; easing.type: Easing.OutQuint }

        }
    ]

}
