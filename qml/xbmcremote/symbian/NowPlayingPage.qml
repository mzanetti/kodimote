import QtQuick 1.1
import com.nokia.symbian 1.1
import Xbmc 1.0

Page {
    id: mainPage
    tools: nowPlayingToolbar
    anchors.margins: appWindow.pageMargin
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: player.currentItem

    property string orientation: width > height ? "landscape" : "portrait"

    onPlayerChanged: {
        if(player === null) {
            do {
            }while(pageStack.pop() !== mainPage);
        }
    }

    property bool timerActive: platformWindow.visible && mainPage.status == PageStatus.Active

    onTimerActiveChanged: { player.timerActive = timerActive }

    ToolBarLayout {
        id: nowPlayingToolbar
        ToolButton { iconSource: "icons/tools-columns.png";
            onClicked: {
                pageStack.pop()
            }
        }
        ToolButton {
            iconSource: "icons/tools-shuffle" + (xbmc.activePlayer.shuffle ? "-enabled" : "") + ".png"
            onClicked: {
                xbmc.activePlayer.shuffle = ! xbmc.activePlayer.shuffle
            }
        }
        ToolButton { iconSource: "toolbar-dialer";
            onClicked: {
                if(xbmc.picturePlayerActive) {
                    pageStack.replace(pictureControlsPage);
                } else {
                    pageStack.replace(keypadPage);
                }
            }
        }
        ToolButton {
            iconSource: xbmc.activePlayer.repeat === Player.RepeatOne ? "icons/tools-repeat-one.png" : (xbmc.activePlayer.repeat === Player.RepeatAll ? "icons/tools-repeat-all.png" : "icons/tools-repeat.png")
            onClicked: {
                if(xbmc.activePlayer.repeat === Player.RepeatNone) {
                    xbmc.activePlayer.repeat = Player.RepeatOne;
                } else if(xbmc.activePlayer.repeat === Player.RepeatOne) {
                    xbmc.activePlayer.repeat = Player.RepeatAll;
                } else {
                    xbmc.activePlayer.repeat = Player.RepeatNone;
                }
            }
        }
        ToolButton { iconSource: "toolbar-list";
            onClicked: {
                var component = Qt.createComponent("PlaylistPage.qml")
                if (component.status === Component.Ready) {
                    pageStack.push(component);
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("player is " + player)
        console.log("playlist is " + playlist)
    }

    Grid {
        anchors.fill: parent
        columns: mainPage.orientation == "portrait" ? 1 : 2
        spacing: appWindow.pageMargin

        Item {
            id: imageItem
            height: mainPage.orientation == "portrait" ? parent.height - textItem.height - parent.spacing : parent.height
            width: mainPage.orientation == "portrait" ? parent.width : height

            Rectangle {
                id: defaultFanart
                anchors.fill: parent
                color:  "black"

                Text {
                    anchors.fill: parent
                    textFormat: Text.StyledText
                    text: "<b>" + currentItem.album + "</b> " + currentItem.album + " " + currentItem.album
                    wrapMode: Text.WrapAnywhere
                    color: "lightblue"
                    font.pixelSize: 85
                    font.capitalization: Font.AllUppercase
                    clip: true
                    visible: currentItem.thumbnail.length === 0 || currentItem.thumbnail === "DefaultAlbumCover.png" || currentItem.thumbnail === "DefaultVideoCover.png"
                }
            }

            Image {
                anchors.fill: parent
                source: currentItem.thumbnail.length === 0 ? "" : xbmc.vfsPath + currentItem.thumbnail
                fillMode: Image.PreserveAspectFit

                onSourceChanged: print("thumbnail source is now:" + currentItem.thumbnail)
            }
        }

        Item {
            id: textItem
            width: mainPage.orientation == "portrait" ? parent.width : parent.width - imageItem.width - parent.spacing
            height: mainPage.orientation == "portrait" ? 200 : parent.height
            Label {
                id: currentTime
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: player.time
            }

            Label {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: currentItem.durationString
            }

            Row {
                id: controlButtons
                anchors { left:parent.left; right: parent.right; bottom: progressBar.top }
                anchors.leftMargin: 20
                anchors.bottomMargin: 10
                anchors.rightMargin: 20
                spacing: (width - (backButton.width * 3)) / 2
                ToolButton { id: backButton; iconSource: "toolbar-mediacontrol-backwards"
                    onClicked: player.skipPrevious();
                }
                ToolButton { iconSource: player.state == "playing" ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play"
                    onClicked: player.playPause();
                }
                ToolButton { iconSource: "toolbar-mediacontrol-forward"
                    onClicked: player.skipNext();
                }
            }

            ProgressBar {
                id: progressBar
                anchors { left: parent.left; right: parent.right; bottom: currentTime.top }
                anchors.bottomMargin: 10
                width: 300
                minimumValue: 0
                maximumValue: 100
                value: player.percentage

                Rectangle {
                    color: "white"
                    rotation: 45
                    width: 10
                    height: 10
                    anchors.horizontalCenter: progressBarLabel.horizontalCenter
                    anchors.verticalCenter: progressBarLabel.bottom
                    visible: progressBarLabel.visible
                }

                Rectangle {
                    id: progressBarLabel
                    color: "white"
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 40
                    height: 40
                    width: progressBarLabelText.width + 20
                    radius: 5
                    visible: progressBarMouseArea.pressed

                    Label {
                        id: progressBarLabelText
                        anchors.centerIn: parent
                        color: "black"
                    }
                }

                MouseArea {
                    id: progressBarMouseArea
                    anchors.fill: progressBar
                    anchors.topMargin: -10
                    anchors.bottomMargin: -10

                    onMouseXChanged: {
                        // Center label on mouseX
                        progressBarLabel.x = mouseX - progressBarLabel.width / 2;

                        // Calculate time under mouseX
                        var targetTime = (progressBarLabel.x + progressBarLabel.width / 2) * currentItem.durationInSecs / progressBar.width;
                        targetTime = Math.min(targetTime, currentItem.durationInSecs);
                        targetTime = Math.max(targetTime, 0);

                        // Translate to human readable time
                        var hours = Math.floor(targetTime / 60 / 60);
                        var minutes = Math.floor(targetTime / 60) % 60;
                        if(minutes < 10) minutes = "0" + minutes;
                        var seconds = Math.floor(targetTime) % 60;
                        if(seconds < 10) seconds = "0" + seconds;

                        // Write into the label
                        if(currentItem.durationInSecs < 60 * 60) {
                            progressBarLabelText.text = minutes + ":" + seconds;
                        } else {
                            progressBarLabelText.text = hours + ":" + minutes + ":" + seconds;
                        }
                    }
                    onReleased: {
                        player.seek(mouseX * 100 / width)
                    }
                }
            }

            Row {
                id: albumRow
                anchors.bottom: controlButtons.top
                height: albumLabel.height
                anchors.left: parent.left
//                anchors.bottomMargin: 10
                Label {
                    id: albumLabel
                    text: xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season : qsTr("Rating:") + " ")
                }
                property int starCount: currentItem.rating > 10 ? Math.round(currentItem.rating / 20) : Math.round(currentItem.rating / 2)
                Repeater {
                    model: parent.starCount
                    Image {
                        visible: currentItem.type === "movie"
                        source: "icons/rating-star.png"
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Image {
                        visible: currentItem.type == "movie"
                        source: "icons/rating-star.png"
                        opacity: .4
                    }
                }
            }

            Label {
                id: artistLabel
//                anchors.bottomMargin: 10
                anchors.left: parent.left
                anchors.bottom: albumRow.top
                anchors.right: parent.right
                text: xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year)
            }
            Button {
                id: infoButton
                text: "i"
                width: height
                anchors.top: trackNumLabel.bottom
                anchors.right: parent.right

                onClicked: {
                    //listView.model.fetchItemDetails(listView.currentIndex)
                    pageStack.push(Qt.createComponent(Qt.resolvedUrl("NowPlayingDetails.qml")));
                }
            }

            Label {
                id: trackLabel
                anchors.left: parent.left
                //anchors.right: trackNumLabel.right
                width: parent.width - trackNumLabel.width
                anchors.bottom: artistLabel.top
//                anchors.bottomMargin: 10
                text: currentItem.title
                elide: Text.ElideRight
                font.bold: true
            }
            Label {
                id: trackNumLabel
                anchors.right: parent.right
                anchors.bottom: artistLabel.top
//                anchors.bottomMargin: 10
                text: playlist.currentTrackNumber + "/" + playlist.count
            }
        }
    }
}
