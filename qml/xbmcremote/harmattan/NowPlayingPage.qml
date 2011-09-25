import QtQuick 1.1
import com.meego 1.0
//import Xbmc 1.0

Page {
    id: mainPage
    tools: nowPlayingToolbar
    anchors.margins: appWindow.pageMargin
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: playlist.currentItem

    property string orientation: width > height ? "landscape" : "portrait"

    onPlayerChanged: {
        if(player === null) {
            do {
            }while(pageStack.pop() !== mainPage);
        }
    }

    ToolBarLayout {
        id: nowPlayingToolbar
        ToolIcon { platformIconId: "toolbar-column";
             anchors.left: parent===undefined ? undefined : parent.left
             onClicked: {
                 pageStack.pop()
             }
        }
        ToolIcon { platformIconId: "toolbar-dialer";
             anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
             onClicked: {
                 if(xbmc.picturePlayerActive) {
                     pageStack.replace(pictureControlsPage);
                 } else {
                     pageStack.replace(keypadPage);
                 }
             }
        }
        ToolIcon { platformIconId: "toolbar-view-menu";
             anchors.right: parent===undefined ? undefined : parent.right
             onClicked: {
                 var component = Qt.createComponent("PlaylistPage.qml")
                 if (component.status == Component.Ready) {
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
                    text: "<b>" + currentItem.album + "</b> " + currentItem.album + " " + currentItem.album
                    wrapMode: Text.WrapAnywhere
                    color: "lightblue"
                    font.pixelSize: 85
                    font.capitalization: Font.AllUppercase
                    clip: true
                }
            }

            Image {
                anchors.fill: parent
                source: currentItem.thumbnail.empty ? "" : xbmc.vfsPath + currentItem.thumbnail
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
                text: player.time
            }

            Label {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: currentItem.durationString
            }

            ProgressBar {
                id: progressBar
                anchors { left: parent.left; right: parent.right; bottom: currentTime.top }
                anchors.bottomMargin: 10
                width: 300
                minimumValue: 0
                maximumValue: 100
                value: player.percentage
            }

            ButtonRow {
                id: controlButtons
                anchors {left:parent.left; right: parent.right; bottom: progressBar.top }
                anchors.bottomMargin: 20
                ToolIcon { platformIconId: "toolbar-mediacontrol-previous"
                    anchors.left: parent.left
                    onClicked: player.skipPrevious();
                }
                ToolIcon { platformIconId: player.state == "playing" ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play"
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: player.playPause();
                }
                ToolIcon { platformIconId: "toolbar-mediacontrol-next"
                    anchors.right: parent.right
                    onClicked: player.skipNext();
                }
            }

            Row {
                id: albumRow
                anchors.bottom: controlButtons.top
                height: albumLabel.height
                anchors.left: parent.left
                anchors.bottomMargin: 20
                Label {
                    id: albumLabel
                    text: xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? "Season: " + currentItem.season : "Rating: ")
                }
                Repeater {
                    model: Math.floor(currentItem.rating / 2)
                    Image {
                        source: "image://theme/meegotouch-indicator-rating-star"
                    }
                }
            }

            Label {
                id: artistLabel
                anchors.bottomMargin: 10
                anchors.left: parent.left
                anchors.bottom: albumRow.top
                anchors.right: parent.right
                text: xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : "Year: " + currentItem.year)
            }
            Label {
                id: trackLabel
                anchors.left: parent.left
                anchors.right: trackNumLabel.right
                anchors.bottom: artistLabel.top
                anchors.bottomMargin: 20
                text: currentItem.title
                font.bold: true
            }
            Label {
                id: trackNumLabel
                anchors.right: parent.right
                anchors.bottom: artistLabel.top
                anchors.bottomMargin: 20
                text: playlist.currentTrackNumber + "/" + playlist.count
            }
        }
    }    
}
