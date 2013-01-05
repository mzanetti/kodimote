import QtQuick 1.1
import com.nokia.meego 1.0
//import Xbmc 1.0

Page {
    id: mainPage
    tools: nowPlayingToolbar
    anchors.margins: appWindow.pageMargin
    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()

    ToolBarLayout {
        id: nowPlayingToolbar
        visible: false
        ToolIcon { platformIconId: "toolbar-back";
             anchors.left: parent===undefined ? undefined : parent.left
             onClicked: pageStack.pop();
        }
//        ToolIcon { platformIconId: "toolbar-mediacontrol-play";
//             anchors.right: parent===undefined ? undefined : parent.right
//             onClicked: pageStack.push(nowPlayingPage)
//        }
//        ToolIcon { platformIconId: "toolbar-view-menu";
//             anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
//             onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
//        }
    }

    Component.onCompleted: {
        console.log("player is " + player)
        console.log("playlist is " + playlist)
    }

    ListView {
        id: listView
        anchors {left: parent.left; right: parent.right; top: listHeader.bottom; bottom: parent.bottom }
        model: playlist
        property int currentSelected

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -mainPage.anchors.leftMargin
                anchors.rightMargin: -mainPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Row {
                anchors.fill: parent

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: title
                        font.weight: Font.Bold
                        font.pixelSize: 26
                        width: listView.width - durationLabel.width
                        elide: Text.ElideRight
                        states: [
                            State {
                                name: "highlighted"; when: index === listView.model.currentTrackNumber - 1
                                PropertyChanges { target: mainText; color: theme.inverted ? "#21496b" : "#318ee7" }
                            }

                        ]
                    }

                    Label {
                        id: subText
                        text: subtitle
                        font.weight: Font.Light
                        font.pixelSize: 24
                        color: theme.inverted ? "#7b797b" : "#848684"
                        width: listView.width - durationLabel.width
                        elide: Text.ElideRight
                        visible: text != ""
                    }
                }
            }

            Label {
                id: durationLabel
                text: duration
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background

                onPressed: listView.currentSelected = index

                onPressAndHold: {
                    longTapMenu.open();
                }

                onClicked: {
                    player.playItem(index);
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: listView
    }
    Image {
        id: listHeader
        anchors {left: parent.left; top: parent.top; right: parent.right }
        anchors.leftMargin: -mainPage.anchors.leftMargin
        anchors.rightMargin: -mainPage.anchors.rightMargin
        anchors.topMargin: -mainPage.anchors.topMargin
        source: "image://theme/meegotouch-view-header-fixed" + (theme.inverted ? "-inverted" : "")
        Label {
            anchors.margins: mainPage.anchors.margins
            anchors.fill: parent
            anchors.verticalCenter: listHeader.verticalCenter
            font.pixelSize: 28
            text: playlist.title
        }
    }

    ContextMenu {
        id: longTapMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Play")
                onClicked: {
                    player.playItem(listView.currentSelected)
                }
            }
            MenuItem {
                text: qsTr("Remove from playlist")
                onClicked: {
                    playlist.removeItem(listView.currentSelected)
                }
            }
            MenuItem {
                text: qsTr("Clear playlist")
                onClicked: {
                    playlist.clear()
                }
            }
        }
    }

}
