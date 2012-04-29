import QtQuick 1.1
import com.nokia.symbian 1.1
//import Xbmc 1.0

Page {
    id: browserPage
    tools: toolBarBack
    property alias model: listView.model

    // Delete the model when leaving the page
    Component.onDestruction: {
        console.log("model is " + browserPage.model)
        model.exit();
    }

    ToolBarLayout {
        id: toolBarBack
        visible: false
        ToolButton {
            iconSource: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                if(listView.currentItem && listView.currentItem.state == "expanded" ) {
                    listView.currentItem.state = "collapsed"
                } else {
                    pageStack.pop();
                }
            }
        }
        ToolButton {
            iconSource: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                if(xbmc.picturePlayerActive) {
                    pageStack.push(pictureControlsPage);
                } else {
                    pageStack.push(keypadPage);
                }
            }
        }
        ToolButton {
            iconSource: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.push(nowPlayingPage)
        }
    }

    // The delegate for each section header
    Component {
        id: sectionHeading
        Row {
            width: listView.width
            height: 20
            Item {
                width: listView.width - 25
                height: parent.height
                Rectangle {
                    width: parent.width - 50
                    height: 1
                    color: "#7b797b"
                    anchors.centerIn: parent
                }
            }

            Label {
                text: section
                font.bold: true
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
                color: "#7b797b"
            }
        }
    }

    ListHeader {
        id: listHeader
        text: model !== undefined ? model.title : ""
    }


    ListView {
        id: listView
        anchors {left: parent.left; top: listHeader.bottom; right: parent.right; bottom: parent.bottom }
        //snapMode: ListView.SnapToItem
        //        header: listHeader
        highlightFollowsCurrentItem: true
        cacheBuffer: 860* 3
        clip: true

        delegate:  Item {
            id: listItem
            height: 60
            width: parent.width
            ListView.delayRemove: thumbnailImage.status == Image.Loading
            state: "collapsed"

            MouseArea {
                id: mouseArea
                anchors.fill: background

                onPressed: listView.currentIndex = index

                onPressAndHold: {
                    if(listView.model.hasDetails()) {
                        listView.model.fetchItemDetails(listView.currentIndex)
                        listItem.state = "expanded"
                    } else if(playable) {
                        longTapMenu.open();
                    }
                }

                onClicked: {
                    if(listItem.state == "expanded") {
                        listItem.state = "collapsed"
                    } else {
                        if(filetype === "directory") {
                            var component = Qt.createComponent("BrowserPage.qml")
                            if (component.status === Component.Ready) {
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

            Rectangle {
                id: background
                anchors.fill: parent
                // Fill page borders
                //                anchors.leftMargin: -browserPage.anchors.leftMargin
                //                anchors.rightMargin: -browserPage.anchors.rightMargin
                visible: mouseArea.pressed
                color: "white"
                opacity: .5
            }

            Rectangle {
                id: highlightBar
                color: "#0067bd"
                height: parent.height - 4
                anchors.verticalCenter: parent.verticalCenter
                width: 8
                anchors.left: parent.left
                visible: playcount === 0
            }

            Image {
                id: thumbnailImage
                height: 58
                anchors.left: highlightBar.right
                anchors.leftMargin: 2
                anchors.top: parent.top
                anchors.topMargin: 1
                fillMode: Image.PreserveAspectFit
                smooth: false
                source: settings.useThumbnails ? thumbnail : ""
                sourceSize.height: 58
                visible: settings.useThumbnails
            }

            Row {
                id: itemRow
                anchors {left: parent.left; top: parent.top; right: parent.right }
                height: 60
                anchors.leftMargin: thumbnailImage.width + 15
                anchors.rightMargin: 5

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: title
                        font.weight: Font.Bold
                        width: itemRow.width - (arrow.visible ? arrow.width : 0)
                        elide: Text.ElideRight
                    }

                    Label {
                        id: subText
                        text: subtitle
                        font.weight: Font.Light
                        color: "#7b797b"
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
                anchors.leftMargin: 15
                width: parent.width - 20

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

            ToolButton {
                id: arrow
                //source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                iconSource: "toolbar-next"
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
                visible: filetype === "directory" && listItem.state == "collapsed" ? true : false;
                opacity: .5
            }


            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: listItem; height: listView.height; clip: true }
                    //                    PropertyChanges { target: listView; snapMode: ListView.SnapOneItem }
                    PropertyChanges { target: background; visible: false }
                    PropertyChanges { target: expandedContent; opacity: 1 }
                    PropertyChanges { target: contentLoader; source: "ItemDetails.qml" }
                    PropertyChanges { target: listView; interactive: false; contentY: 60 * listView.currentIndex }
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
        // section.delegate: model.parentModel() != null ? sectionHeading : undefined

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
            color: "white"
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
        height: 100

        Rectangle {
            anchors.fill: parent
            color: "white"
        }
        Label {
            id: scrollIndicatorLabel
            anchors.fill: scrollIndicator
            verticalAlignment: Text.AlignVCenter
            anchors.margins: 10
            color: "black"
            font.pixelSize: 56
        }
    }

    Menu {
        id: longTapMenu
        visualParent: pageStack
        MenuLayout {
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

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
//        platformStyle: BusyIndicatorStyle { size: "large" }
        running: model !== undefined ? model.busy : true
        visible: running
    }

}
