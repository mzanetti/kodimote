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
        //        header: listHeader
        property int currentSelected


        delegate:  Item {
            id: listItem
            height: 88
            width: parent.width
            ListView.delayRemove: thumbnailImage.status == Image.Loading

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
                height: parent.height - 2
//                width: height
                fillMode: Image.PreserveAspectFit
                smooth: false
                source: settings.useThumbnails ? thumbnail : ""
                sourceSize.height: parent.height - 2
                visible: settings.useThumbnails
            }

            Row {
                id: itemRow
                anchors.fill: parent
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
        section.property: "sortingTitle"
        section.criteria: ViewSection.FirstCharacter
//        section.delegate: sectionHeading
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
//        anchors.leftMargin: -browserPage.anchors.leftMargin
//        anchors.rightMargin: -browserPage.anchors.rightMargin
//        anchors.topMargin: -browserPage.anchors.topMargin
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
