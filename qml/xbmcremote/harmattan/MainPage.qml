import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    tools: toolBarEntry
    anchors.margins: appWindow.pageMargin

    ListModel {
        id: mainMenuModel
        ListElement {
            title: "Music"
            icon: "icon-m-content-audio"
            subtitle: ""
        }
        ListElement {
            title: "Videos"
            icon: "icon-m-content-videos"
            subtitle: ""
        }
        ListElement {
            title: "Pictures"
            icon: "icon-m-content-image"
            subtitle: ""
        }
//        ListElement {
//            title: "Video Files"
//            icon: "content-videos"
//            subtitle: ""
//        }
    }


    ListView {
        id: listView
        anchors.fill: parent
//        header: headerLayout
        model: mainMenuModel
        spacing: 20
        property int currentSelected

        delegate:  Item {
            id: listItem
            height: 150
            width: parent.width
            property string mode: "library"
            clip: true

            Rectangle {
                id: background
                anchors.fill: parent
                opacity: theme.inverted ? 0.1 : 0.05
                // Fill page borders
//                anchors.leftMargin: -mainPage.anchors.leftMargin
//                anchors.rightMargin: -mainPage.anchors.rightMargin
                color: theme.inverted ? "white" : "black"
                radius: 20
            }

            BorderImage {
                id: backgroundPressed
                anchors.fill: parent
                // Fill page borders
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }


            Row {
                id: textRow
                anchors.fill: parent
                height: 150
                spacing: 20
                anchors.margins: 20


                Image {
                    id: toolIcon
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/" + icon + (theme.inverted ? "-inverse" : "")
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: title
                        font.weight: Font.Bold
                        font.pixelSize: 26
                    }

                    Label {
                        id: subText
                        text: listItem.mode == "library" ? "Library" : "Files"
                        font.weight: Font.Light
                        font.pixelSize: 24
                        color: theme.inverted ? "#7b797b" : "#848684"
                        visible: text != ""
                    }
                }
            }
//            Image {
//                id: arrow
//                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
//                anchors.right: parent.right;
//                anchors.verticalCenter: parent.verticalCenter
//            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onPressed: listView.currentSelected = index;

                onPressAndHold: {
                    if(index == 0 || index  == 1) {
                        longTapMenu.open();
                    }
                }

                onClicked: {
                    var component = Qt.createComponent("BrowserPage.qml")
                    var newModel;
                    if (component.status == Component.Ready) {
                        switch(index) {
                        case 0:
                            if(listItem.mode == "library") {
                                newModel = xbmc.audioLibrary();
                            } else {
                                newModel = xbmc.shares("music");
                            }
                            break
                        case 1:
                            if(listItem.mode == "library") {
                                newModel = xbmc.videoLibrary();
                            } else {
                                newModel = xbmc.shares("video");
                            }
                            break;
                        case 2:
                            newModel = xbmc.shares("pictures");
                            console.log("created model: " + newModel);
                            break;
//                        case 3:
//                            newModel = xbmc.shares("video");
//                            console.log("created model: " + newModel);
//                            break;
                        }
                        console.log("setting model: " + newModel);
                        pageStack.push(component, {model: newModel});
                    } else {
                            console.log("Error loading component:", component.errorString());
                    }
                }
            }



            ButtonRow {
                anchors.top: textRow.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 20
//                visible: listItem.state == "expanded"
                Button {
                    text: "Library"
                    onClicked: {
                        listItem.state = "normal"
                        listItem.mode = "library"
                    }
                }
                Button {
                    text: "Files"
                    onClicked: {
                        listItem.state = "normal"
                        listItem.mode = "files"
                    }
                }
            }

            states:  [
                State {
                    name: "expanded"
                    PropertyChanges { target: listItem; height: 250 }
                }

            ]

            Behavior on height {
                NumberAnimation { easing.type: Easing.Linear; duration: 200 }
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
                text: "Show files"
                visible: listView.currentItem.mode != "files"
                onClicked: {
                    listView.currentItem.mode = "files"
                }
            }
            MenuItem {
                text: "Show library"
                visible: listView.currentItem.mode != "library"
                onClicked: {
                    listView.currentItem.mode = "library"
                }
            }
            MenuItem {
                text: "Rescan library"
                onClicked: {
                    var lib = xbmc.audioLibrary();
                    switch(listView.currentSelected) {
                    case 0:
                        lib = xbmc.audioLibrary();
                        break;
                    case 1:
                        lib = xbmc.videoLibrary();
                        break;
                    }

                    lib.scanForContent();
                    lib.exit();
                }
            }
        }
    }

}
