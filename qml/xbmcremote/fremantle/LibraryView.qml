import Qt 4.7

Item {
    id: libraryView
    anchors.fill: parent

    signal goBack

    property alias library: list.model
    property alias view: list

    BrowsingTopBar {
        id: topBar
        anchors.left: parent.left
        anchors.top: parent.top
        currentDir: library.title
        currentIcon: library.parentModel() !== null ? "images/HomeBack.png" : "images/HomeIcon.png"

//        onGoUp: {
//            if(library.parentModel() !== null) {
//                library = library.exit()
//            }
//        }
        onGoBack: {
            if(library.parentModel() !== null) {
                library = library.exit()
            } else {
                libraryView.goBack()
            }
        }
    }

    BorderImage {
        id: listBackground
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15
        anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: topBar.bottom }
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
//        state: library.state
        source: "images/ContentPanel.png"

        ListView {
            id: list
            anchors {left: parent.left; right: parent.right; bottom: parent.bottom; top: parent.top}
            anchors.margins: 25
            //        anchors.rightMargin: 20
            clip: true
            model: library
            property int selectedIndex: -1

            delegate: Item {
                width: parent.width
                height: 64
                Image {
                    anchors.fill: parent
                    source: index == list.selectedIndex ? "images/MenuItemFO.png" : "images/MenuItemNF.png"
                }

                Text {
                    color: "white"
                    text: title
                    font.pixelSize: 28
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: contextMenu.state != "visible"

                    onPressed: list.selectedIndex = index

                    onPressAndHold: {
                        // Hack: if we clear the model, the anchors are screwed for some reason. To prevent this
                        // we unset the model and set it back after populating it
                        contextMenu.model = null
                        contextMenuModel.clear();
                        if(playable) {
                            contextMenuModel.append({ "entryId": 0, "menuEntry": "Play"})
                            contextMenuModel.append({ "entryId": 1, "menuEntry": "Add to playlist"})
                        }
                        if(xbmcBrowser.viewState === "library" && list.model.parentModel() === null) {
                            contextMenuModel.append({ "entryId": 2, "menuEntry": "Rescan library"})
                        }
                        if(contextMenuModel.count > 0) {
                            contextMenu.state = "visible"
                        }
                        contextMenu.model = contextMenuModel
                    }

                    onClicked: {
                        if(filetype=="directory") {
                            var newModel = list.model.enterItem(index);
                            console.log("newModel: " + newModel);
                            list.model = newModel;
                        } else {
                            list.model.playItem(index);
                        }
                    }
                }
            }

            Keys.onPressed: {
                console.log("key pressed");
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

    ListModel {
        id: contextMenuModel
        ListElement { entryId: 0; menuEntry: "Play"}
        ListElement { entryId: 1; menuEntry: "Add to playlist"}
        ListElement { entryId: 2; menuEntry: "Rescan library"}
    }

    ContextMenu {
        id: contextMenu

        model: contextMenuModel

        onClicked: {
            switch(index) {
            case 0:
                list.model.playItem(list.selectedIndex);
                break;
            case 1:
                list.model.addToPlaylist(list.selectedIndex);
                break;
            case 2:
                list.model.scanForContent();
                break;
            }
        }
    }



//    Behavior on opacity {
//        NumberAnimation { easing.type: Easing.InQuint; duration: 300 }
//    }

    states: [
        State {
            name: "hidden"; when: !visible
            PropertyChanges { target: listBackground; opacity: 0}
            PropertyChanges { target: topBar; opacity: 0; anchors.leftMargin: -topBar.width}
        },
        State {
            name: "visible"; when: visible
            PropertyChanges { target: listBackground; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.InQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  300; easing.type: Easing.OutQuad}
        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.OutQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  200; easing.type: Easing.InQuad}
            NumberAnimation { target: topBar; property: "opacity"; duration: 300; easing.type: Easing.OutQuad}
        }
    ]

}
