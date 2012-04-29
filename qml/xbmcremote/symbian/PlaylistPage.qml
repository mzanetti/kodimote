import QtQuick 1.1
import com.nokia.symbian 1.1
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
        ToolButton {
            iconSource: "toolbar-back";
             anchors.left: parent===undefined ? undefined : parent.left
             onClicked: pageStack.pop();
        }
    }

    Component.onCompleted: {
        console.log("player is " + player)
        console.log("playlist is " + playlist)
    }

    ListHeader {
        id: listHeader
        text: playlist.title
    }

    ListView {
        id: listView
        anchors {left: parent.left; right: parent.right; top: listHeader.bottom; bottom: parent.bottom }
        model: playlist
        property int currentSelected

        delegate:  Item {
            id: listItem
            height: 60
            width: parent.width - 30
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -listView.anchors.leftMargin
                anchors.rightMargin: -listView.anchors.rightMargin
                visible: mouseArea.pressed
                color: "white"
                opacity: .5
            }

            Row {
                anchors.fill: parent

                Column {
                    anchors.verticalCenter: parent.verticalCenter


                    Label {
                        id: mainText
                        text: title
                        font.weight: Font.Bold
                        width: listItem.width - 5 - durationLabel.width
                        elide: Text.ElideRight
                    }

                    Label {
                        id: subText
                        text: subtitle !== undefined ? subtitle : ""
                        font.weight: Font.Light
                        color: "#7b797b"
                        width: listItem.width - 5 - durationLabel.width
                        elide: Text.ElideRight
                        visible: text !== ""
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
        }
    }

}
