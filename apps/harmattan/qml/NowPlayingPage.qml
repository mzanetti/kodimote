import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Page {
    id: mainPage
    tools: nowPlayingToolbar
    anchors.margins: appWindow.pageMargin
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    property string orientation: width > height ? "landscape" : "portrait"

    onPlayerChanged: {
        if(player === null) {
            if(mainPage.status === PageStatus.Active) {
                pageStack.pop();
            }
        }
    }

    property bool timerActive: platformWindow.visible && mainPage.status == PageStatus.Active

    onTimerActiveChanged: { player.timerActive = timerActive }

   Connections {
       target: xbmc
       onStateChanged: {
           print("!*!*! state:", xbmc.state);
       }
   }

    ToolBarLayout {
        id: nowPlayingToolbar
        ToolIcon { platformIconId: "toolbar-column";
            onClicked: {
                pageStack.pop()
            }
        }
        ToolIcon {
            platformIconId: xbmc.state == "video" ? "toolbar-volume" : ""
            iconSource: {
                if (xbmc.state == "audio") {
                    return "icons/shuffle" + (player && player.shuffle ? "-on" : "-off") + (theme.inverted ? "-black" : "-white") + ".png";
                } else {
                    return ""
                }
            }
            onClicked: {
                if (xbmc.state == "audio") {
                    player.shuffle = ! player.shuffle
                } else if (xbmc.state == "video") {
                    var sheetObj = selectMediaPropertySheet.createObject(mainPage)
                    sheetObj.model = player.audiostreams;
                    sheetObj.currentIndex = player.currentAudiostream;
                    sheetObj.accepted.connect(function() {
                                                  player.currentAudiostream = sheetObj.currentIndex;
                                              })
                    sheetObj.open();
                }
            }
        }
        ToolIcon { platformIconId: "toolbar-dialer";
            onClicked: {
                if(xbmc.picturePlayerActive) {
                    pageStack.replace(pictureControlsPage);
                } else {
                    pageStack.replace(keypadPage);
                }
            }
        }
        ToolIcon {
            iconSource: {
                if (xbmc.state == "audio") {
                    var name = "icons/repeat";
                    if (player && player.repeat === Player.RepeatOne) {
                        name += "-one";
                    } else if (player && player.repeat === Player.RepeatAll) {
                        name += "-all";
                    } else {
                        name += "-none";
                    }

                    if (theme.inverted) {
                        name += "-black";
                    } else {
                        name += "-white";
                    }
                    name += ".png";
                    return name;
                } else if (xbmc.state == "video") {
                    if (player.currentSubtitle >= 0) {
                        return "image://theme/icon-m-camera-flash-red-eye-pressed";
                    } else {
                        return "image://theme/icon-m-camera-flash-red-eye-choice";
                    }
                }

                return "";
            }
            onClicked: {
                if (!player) {
                    return;
                }

                if (xbmc.state == "audio") {
                    if(player.repeat === Player.RepeatNone) {
                        player.repeat = Player.RepeatOne;
                    } else if(player.repeat === Player.RepeatOne) {
                        player.repeat = Player.RepeatAll;
                    } else {
                        player.repeat = Player.RepeatNone;
                    }
                } else if (xbmc.state == "video") {
                    var sheetObj = selectMediaPropertySheet.createObject(mainPage)
                    sheetObj.model = player.subtitles;
                    sheetObj.currentIndex = player.currentSubtitle;
                    sheetObj.rejectButtonText = qsTr("Off");
                    sheetObj.accepted.connect(function() {
                                                  player.currentSubtitle = sheetObj.currentIndex;
                                              })
                    sheetObj.rejected.connect(function() {
                                                  player.currentSubtitle = -1;
                                              })
                    sheetObj.open();
                }
            }
        }
        ToolIcon { platformIconId: "toolbar-view-menu";
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
            height: mainPage.orientation == "portrait" ? parent.width : parent.height
            width: mainPage.orientation == "portrait" ? parent.width : height
            //            Rectangle {
            //                color: "blue"
            //                anchors.fill: parent
            //            }

            Rectangle {
                id: defaultFanart
                anchors.fill: parent
                color:  "black"

                Text {
                    anchors.fill: parent
                    textFormat: Text.StyledText
                    property string coverText: (!currentItem ? "" : (xbmc.state == "audio" ? currentItem.album : currentItem.title))
                    text: "<b>" + coverText + "</b> " + coverText + " " + coverText
                    wrapMode: Text.WrapAnywhere
                    color: "lightblue"
                    font.pixelSize: 85
                    font.capitalization: Font.AllUppercase
                    clip: true
                    visible: currentItem !== null && (currentItem.thumbnail.length === 0 || currentItem.thumbnail === "DefaultAlbumCover.png" || currentItem.thumbnail === "DefaultVideoCover.png")
                }
            }

            Image {
                anchors.fill: parent
                source: !currentItem || currentItem.thumbnail.length === 0 ? "" : currentItem.thumbnail
                fillMode: Image.PreserveAspectFit

                onSourceChanged: print("thumbnail source is now:" + currentItem.thumbnail)
            }
        }

        Item {
            id: textItem
            width: mainPage.orientation == "portrait" ? parent.width : parent.width - imageItem.width - parent.spacing
            height: mainPage.orientation == "portrait" ? parent.height - imageItem.height - parent.spacing : parent.height
            Label {
                id: currentTime
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: player ? player.time : "00:00"
            }

            Label {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: currentItem ? currentItem.durationString : "00:00"
            }

            PlayerControls {
                id: controlButtons
                anchors.bottom: progressBar.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                anchors.bottomMargin: 20
                player: nowPlayingPage.player
            }

            ProgressBar {
                id: progressBar
                anchors { left: parent.left; right: parent.right; bottom: currentTime.top }
                anchors.bottomMargin: 10
                width: 300
                minimumValue: 0
                maximumValue: 100
                value: player ? player.percentage : 0

                Rectangle {
                    color: theme.inverted ? "white" : "black"
                    rotation: 45
                    width: 10
                    height: 10
                    anchors.horizontalCenter: progressBarLabel.horizontalCenter
                    anchors.verticalCenter: progressBarLabel.bottom
                    visible: progressBarLabel.visible
                }

                Rectangle {
                    id: progressBarLabel
                    color: theme.inverted ? "white" : "black"
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 40
                    height: 40
                    width: progressBarLabelText.width + 20
                    radius: 5
                    visible: progressBarMouseArea.pressed

                    Label {
                        id: progressBarLabelText
                        anchors.centerIn: parent
                        color: theme.inverted ? "black" : "white"
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
                        var progressedWidth = mouseX - progressBar.x;
                        var targetTime = progressedWidth * currentItem.durationInSecs / progressBar.width;
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
                anchors.bottomMargin: 10
                Label {
                    id: albumLabel
                    text: {
                        if (!currentItem) {
                            return "";
                        }

                        if (xbmc.state == "audio") {
                            return currentItem.album;
                        } else {
                            if (currentItem.type === "episode") {
                                return qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode;
                            } else if (currentItem.type === "channel") {
                                return currentItem.title;
                            }
                        }
                        // movies & videos
                        return qsTr("Rating:") + " "
                    }
                }
                property int starCount: !currentItem ? 0 : (currentItem.rating > 10 ? Math.floor(currentItem.rating / 20) : Math.floor(currentItem.rating / 2))
                Repeater {
                    model: parent.starCount
                    Image {
                        visible: currentItem.type === "movie"
                        source: theme.inverted ? "image://theme/meegotouch-indicator-rating-inverted-background-star" : "image://theme/meegotouch-indicator-rating-star"
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Image {
                        visible: currentItem !== null && currentItem.type === "movie"
                        source: theme.inverted ? "image://theme/meegotouch-indicator-rating-background-star" : "image://theme/meegotouch-indicator-rating-background-star"
                    }
                }
            }

            Label {
                id: artistLabel
                anchors.bottomMargin: 6
                anchors.left: parent.left
                anchors.bottom: albumRow.top
                anchors.right: infoButton.left
                text: !currentItem ? "" : (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year))
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
                anchors.bottomMargin: 6
                text: currentItem ? currentItem.title : ""
                elide: Text.ElideRight
                font.bold: true
            }
            Label {
                id: trackNumLabel
                anchors.right: parent.right
                anchors.bottom: artistLabel.top
                anchors.bottomMargin: 6
                text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"
            }
        }
    }

    Component {
        id: selectMediaPropertySheet

        Sheet {
            id: sheetRoot
            rejectButtonText: qsTr("Cancel")
            acceptButtonText: qsTr("OK")
            acceptButton.enabled: currentIndex >= 0

            property alias model: sheetList.model
            property alias currentIndex: sheetList.currentIndex

            signal itemSelected(int index)

            onRejected: {
                sheetRoot.destroy();
            }
            onAccepted: {
                sheetRoot.destroy();
            }

            content: ListView {
                id: sheetList
                anchors.fill: parent
                anchors.margins: 10
                model: sheetRoot.model
                highlightFollowsCurrentItem: true

                highlight: Rectangle {
                    width: sheetList.width
                    height: 88
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: theme.inverted ? "#4269a5" : "#3996e7" }
                        GradientStop { position: 1.0; color: theme.inverted ? "#29599c" : "#1082de" }
                    }

                    //color: theme.inverted ? "#1d87e0"
                }

                delegate: Item {
                    width: parent.width
                    height: 88

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sheetList.currentIndex = index;
                        }
                    }

                    Label {
                        id: mainText
                        anchors {left: parent.left; right: parent.right; margins: 10; verticalCenter: parent.verticalCenter }
                        text: modelData
                        font.weight: Font.Bold
                        font.pixelSize: 26
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
