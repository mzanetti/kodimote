import QtQuick 1.1
import com.meego 1.0
//import Xbmc 1.0

Page {
    id: browserPage
    tools: toolBarBack
    anchors.margins: appWindow.pageMargin
    property QtObject model

    ToolBarLayout {
        id: toolBarBack
        visible: false
        ToolIcon { platformIconId: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: pageStack.pop();
        }
        ToolIcon { platformIconId: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                if(xbmc.picturePlayerActive) {
                    pageStack.push(pictureControlsPage);
                } else {
                    pageStack.push(keypadPage);
                }
            }
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.push(nowPlayingPage)
        }
    }

    Component.onCompleted: {
        console.log("setting model " + model)
        listView.model = model
    }

    Component.onDestruction: {
        console.log("model is " + model)
        model.exit();
    }

    ListView {
        id: listView
        anchors {left: parent.left; top: listHeader.bottom; right: parent.right; bottom: parent.bottom }
        //        header: listHeader
        property int currentSelected


        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -browserPage.anchors.leftMargin
                anchors.rightMargin: -browserPage.anchors.rightMargin
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
                        width: listView.width - arrow.width
                        elide: Text.ElideRight
                    }

                    Label {
                        id: subText
                        text: subtitle
                        font.weight: Font.Light
                        font.pixelSize: 24
                        color: theme.inverted ? "#7b797b" : "#848684"
                        width: listView.width - arrow.width
                        elide: Text.ElideRight
                        visible: text != ""
                    }
                }
            }

            Image {
                id: arrow
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                visible: filetype == "directory" ? true : false;
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background

                onPressed: listView.currentSelected = index

                onPressAndHold: {
                    if(playable) {
                        longTapMenu.open();
                    }
                }

                onClicked: {
                    if(filetype=="directory") {
                        var component = Qt.createComponent("BrowserPage.qml")
                        if (component.status == Component.Ready) {
                            var newModel = browserPage.model.enterItem(index);
                            pageStack.push(component, {model: newModel});
                        } else {
                            console.log("Error loading component:", component.errorString());
                        }
                    } else {
                        browserPage.model.playItem(index);
                    }
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
        anchors.leftMargin: -browserPage.anchors.leftMargin
        anchors.rightMargin: -browserPage.anchors.rightMargin
        anchors.topMargin: -browserPage.anchors.topMargin
        source: "image://theme/meegotouch-view-header-fixed" + (theme.inverted ? "-inverted" : "")
        Label {
            anchors.margins: browserPage.anchors.margins
            anchors.fill: parent
            anchors.verticalCenter: listHeader.verticalCenter
            font.pixelSize: 28
            text: model.title
            elide: Text.ElideLeft
        }
    }

    Menu {
        id: longTapMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Play"
                onClicked: {
                    browserPage.model.playItem(listView.currentSelected)
                }
            }
            MenuItem {
                text: "Add to playlist"
                onClicked: {
                    browserPage.model.addToPlaylist(listView.currentSelected)
                }
            }
        }
    }

}
