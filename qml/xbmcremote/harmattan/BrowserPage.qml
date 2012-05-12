import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Page {
    id: browserPage
    tools: toolBarBack
    property variant model

    ToolBarLayout {
        id: toolBarBack
        visible: false
        ToolIcon { platformIconId: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                if(listView.currentItem && listView.currentItem.state == "expanded" ) {
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
        //filterModel.model = model
    }

    Component.onDestruction: {
        console.log("model is " + model)
        model.exit();
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
                    color: theme.inverted ? "#7b797b" : "#848684"
                    anchors.centerIn: parent
                }
            }

            Label {
                text: section
                font.bold: true
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
                color: theme.inverted ? "#7b797b" : "#848684"
            }
        }
    }

    Item {
        id: searchBar
        anchors.top: listHeader.bottom
        anchors.topMargin: listView.currentItem.state == "expanded" ? -height : 0
        width: parent.width
        //height: 80
        height: expanded ? 80 : 0
        property bool expanded

        Timer {
            id: searchTimer
            interval: 3000
            repeat: false
            running: searchBar.expanded && !searchTextField.activeFocus && filterModel.filter.length === 0

            onTriggered: {
                searchBar.expanded = false;
            }
        }


        Behavior on height {
            NumberAnimation { duration: 1500; easing.type: Easing.OutElastic }
        }
    }

    TextField {
        id: searchTextField
        anchors { left: parent.left; right: parent.right; bottom: listView.top }
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: listView.contentY + (80 - height) / 2
        opacity: searchBar.expanded ? 1 : 0
        inputMethodHints: Qt.ImhNoPredictiveText

        Image {
            id: searchimage
            source: searchTextField.text.length == 0 ? "image://theme/icon-m-common-search" : "image://theme/icon-m-browser-clear"
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                anchors.margins: -20
                onClicked: searchTextField.text = "";
            }

        }

        Behavior on opacity {
            NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
        }

        onTextChanged: {
            filterModel.filter = text;
        }
    }

    XbmcFilterModel {
        id: filterModel
        model: browserPage.model
        caseSensitive: false

        // When the model is filtered, contentY is messed up sometimes
        // Lets unset and reset the model to keep the searchbar in place
        onFilterChanged: {
            listView.model = undefined
            listView.model = filterModel
        }
    }

    ListView {
        id: listView
        anchors {left: parent.left; top: searchBar.bottom; right: parent.right; bottom: parent.bottom }
        //snapMode: ListView.SnapToItem
        //        header: listHeader
        highlightFollowsCurrentItem: true
        cacheBuffer: 88 * 3
        model: filterModel

        property variant headerItem: null
        property bool draggedForSearch: contentY < -100


        onDraggedForSearchChanged: {
            searchBar.expanded = true;
        }

        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width
            ListView.delayRemove: thumbnailImage.status == Image.Loading
            state: "collapsed"

            MouseArea {
                id: mouseArea
                anchors.fill: background

                onPressed: listView.currentIndex = index

                onPressAndHold: {
                    if(browserPage.model.hasDetails()) {
                        browserPage.model.fetchItemDetails(filterModel.mapToSourceIndex(listView.currentIndex))
                        listItem.state = "expanded"
                        print("expanding:", listView.currentIndex, listView.count)
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
                                var newModel = browserPage.model.enterItem(filterModel.mapToSourceIndex(index));
                                newModel.ignoreArticle = settings.ignoreArticle;
                                pageStack.push(component, {model: newModel});
                            } else {
                                console.log("Error loading component:", component.errorString());
                            }
                        } else {
                            browserPage.model.playItem(filterModel.mapToSourceIndex(index));
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
                height: 86
                anchors.left: highlightBar.right
                anchors.leftMargin: 2
                anchors.top: parent.top
                anchors.topMargin: 1
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
                anchors.leftMargin: thumbnailImage.width + 15
                anchors.rightMargin: 5

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
                            browserPage.model.playItem(filterModel.mapToSourceIndex(index))
                        }

                        onAddToPlaylist: {
                            browserPage.model.addToPlaylist(filterModel.mapToSourceIndex(index))
                        }

                        onDownload: {
                            browserPage.model.download(filterModel.mapToSourceIndex(index), "/home/user/MyDocs/");
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
                    PropertyChanges { target: listView; interactive: false; contentY: 88 * listView.currentIndex }
                    PropertyChanges { target: fastScroller; enabled: false }
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
        anchors {top: searchBar.bottom; right: parent.right; bottom: parent.bottom }
        width: 75

        Rectangle {
            id: scrollBackground
            color: theme.inverted ? "white" : "black"
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
            color: theme.inverted ? "white" : "black"
        }
        Label {
            id: scrollIndicatorLabel
            anchors.fill: scrollIndicator
            verticalAlignment: Text.AlignVCenter
            anchors.margins: 10
            color: theme.inverted ? "black" : "white"
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
                text: "Play"
                onClicked: {
                    browserPage.model.playItem(filterModel.mapToSourceIndex(listView.currentIndex))
                }
            }
            MenuItem {
                text: "Add to playlist"
                onClicked: {
                    browserPage.model.addToPlaylist(filterModel.mapToSourceIndex(listView.currentIndex))
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        platformStyle: BusyIndicatorStyle { size: "large" }
        running: model.busy
        visible: model.busy
    }

}
