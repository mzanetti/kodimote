import QtQuick 1.1
import com.nokia.meego 1.0
//import Xbmc 1.0

Page {
    id: browserPage
    tools: toolBarBack
    property QtObject model

    ToolBarLayout {
        id: toolBarBack
        visible: false
        ToolIcon { platformIconId: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                if(listView.currentItem.state == "expanded" ) {
                    listView.currentItem.state = "collapsed"
                } else {
                    pageStack.pop();
                }
            }
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

    // The delegate for each section header
    Component {
        id: sectionHeading
        Rectangle {
            width: listView.width
            height: childrenRect.height
            color: "lightsteelblue"

            Text {
                text: section
                font.bold: true
            }
        }
    }

    ListView {
        id: listView
        anchors {left: parent.left; top: listHeader.bottom; right: parent.right; bottom: parent.bottom }
        //snapMode: ListView.SnapToItem
        //        header: listHeader
        highlightFollowsCurrentItem: true
        cacheBuffer: 88 * 3

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width
            ListView.delayRemove: thumbnailImage.status == Image.Loading

            MouseArea {
                id: mouseArea
                anchors.fill: background

                onPressed: listView.currentIndex = index

                onPressAndHold: {
                    if(playable) {
                        listView.model.fetchItemDetails(listView.currentIndex)
                        //                        listView.positionViewAtIndex(listView.currentIndex, ListView.Beginning)
                        print("positioning at: " + listView.currentIndex + " " + index)
                        listItem.state = "expanded"
//                        listView.contentY = 88 * listView.currentIndex
                        //longTapMenu.open();
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
                        if(listItem.state == "expanded") {
                            listItem.state = "collapsed"
                        } else {
                            browserPage.model.playItem(index);
                        }
                    }
                }
            }

            BorderImage {
                id: background
                anchors.fill: parent
                // Fill page borders
                //                anchors.leftMargin: -browserPage.anchors.leftMargin
                //                anchors.rightMargin: -browserPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }

            Image {
                id: thumbnailImage
                height: 86
                //                width: height
                fillMode: Image.PreserveAspectFit
                smooth: false
                source: settings.useThumbnails ? thumbnail : ""
                sourceSize.height: 86
                visible: settings.useThumbnails
            }

            Row {
                id: itemRow
                anchors {left: parent.left; top: parent.top; right: parent.right }
                height: 88
                anchors.leftMargin: thumbnailImage.width + 5

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: title
                        font.weight: Font.Bold
                        font.pixelSize: 26
                        width: itemRow.width - (arrow.visible ? arrow.width : 0)
                        elide: Text.ElideRight
                    }

                    Label {
                        id: subText
                        text: subtitle
                        font.weight: Font.Light
                        font.pixelSize: 24
                        color: theme.inverted ? "#7b797b" : "#848684"
                        width: itemRow.width - (arrow.visible ? arrow.width : 0)
                        elide: Text.ElideRight
                        visible: text != ""
                    }


                }
            }


            Item {
                id: expandedContent
                anchors {left: parent.left; top: itemRow.bottom; right: parent.right}
                anchors.bottom: parent.bottom
                width: parent.width

                Loader {
                    id: contentLoader
                    anchors.fill: parent
                    onLoaded: print("loaderheight: " + childrenRect.height)

                    Connections {
                        target: contentLoader.item

                        onPlayItem: {
                            print("playItem()!")
                            browserPage.model.playItem(index)
                        }

                        onAddToPlaylist: {
                            browserPage.model.addToPlaylist(index)
                        }
                    }
                }
                Behavior on opacity {
                    NumberAnimation { duration: 300 }
                }
            }

            Image {
                id: arrow
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                visible: filetype == "directory" ? true : false;
            }


            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: listItem; height: listView.height; clip: true }
                    //                    PropertyChanges { target: listView; snapMode: ListView.SnapOneItem }
                    PropertyChanges { target: background; visible: false }
                    PropertyChanges { target: expandedContent; opacity: 1 }
                    PropertyChanges { target: contentLoader; source: "ItemDetails.qml" }
                    PropertyChanges { target: listView; interactive: false; contentY: 88 * listView.currentIndex }
//                    PropertyChanges { target: mouseArea; enabled: false }
                }

            ]

            Behavior on height {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 300
                }
            }
        }
        section.property: "sortingTitle"
        section.criteria: ViewSection.FirstCharacter
        //section.delegate: sectionHeading

        Behavior on contentY {
            NumberAnimation {
                easing.type: Easing.OutQuad
                duration: 300
            }
        }

    }
    ScrollDecorator {
        flickableItem: listView
    }

    MouseArea {
        id:fastScroller
        anchors {top: listHeader.bottom; right: parent.right; bottom: parent.bottom }
        width: 75

        Rectangle {
            id: scrollBackground
            color: "black"
            opacity: 0
            anchors.fill: parent

            Behavior on opacity {
                NumberAnimation {
                    easing.type: Easing.InQuad
                    duration: 200
                }
            }
        }

        onReleased: {
            scrollBackground.opacity = 0;
        }

        onCanceled: {
            scrollBackground.opacity = 0;
        }

        onMouseYChanged: {
            scrollBackground.opacity = 0.2;
            var percent = Math.min(model.count - 1, Math.max(0, Math.round((mouseY) / fastScroller.height * (listView.count - 1))))
            scrollIndicatorLabel.text = model.get(percent, "sortingTitle").substring(0, 1);
            listView.positionViewAtIndex(percent, ListView.Center);
            scrollIndicator.y = Math.min(listView.height - scrollIndicator.height + listView.y, Math.max(listView.y, mouseY + fastScroller.y - scrollIndicator.height / 2))
        }

    }
    Item {
        id: scrollIndicator
        opacity: scrollBackground.opacity * 4
        anchors.left: parent.left
        anchors.right: parent.right
        height: 150

        Rectangle {
            anchors.fill: parent
            color: "black"
        }
        Label {
            id: scrollIndicatorLabel
            anchors.fill: scrollIndicator
            verticalAlignment: Text.AlignVCenter
            anchors.margins: 10
            color: "white"
            font.pixelSize: 64
        }
    }

    Image {
        id: listHeader
        anchors {left: parent.left; top: parent.top; right: parent.right }
        source: "image://theme/meegotouch-view-header-fixed" + (theme.inverted ? "-inverted" : "")
        Label {
            anchors.margins: 10
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
                text: "Details"
                onClicked: {
                    listView.currentItem.state = "expanded"
                    listView.positionViewAtIndex(listView.currentIndex, ListView.Beginning)
                }
            }
            MenuItem {
                text: "Play"
                onClicked: {
                    browserPage.model.playItem(listView.currentIndex)
                }
            }
            MenuItem {
                text: "Add to playlist"
                onClicked: {
                    browserPage.model.addToPlaylist(listView.currentIndex)
                }
            }
        }
    }

}
