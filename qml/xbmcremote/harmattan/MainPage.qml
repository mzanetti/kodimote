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
            icon: "content-music-band"
            subtitle: ""
        }
        ListElement {
            title: "Videos"
            icon: "content-videos"
            subtitle: ""
        }
//        ListElement {
//            title: "Music Files"
//            icon: "content-music-band"
//            subtitle: ""
//        }
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

        delegate:  Item {
            id: listItem
            height: 150
            width: parent.width
            property string mode: "library"
            clip: true

            Rectangle {
                id: background
                anchors.fill: parent
                opacity: 0.05
                // Fill page borders
                anchors.leftMargin: -mainPage.anchors.leftMargin
                anchors.rightMargin: -mainPage.anchors.rightMargin
                color: "black"
                radius: 20
            }

            BorderImage {
                id: backgroundPressed
                anchors.fill: parent
                // Fill page borders
                anchors.leftMargin: -mainPage.anchors.leftMargin
                anchors.rightMargin: -mainPage.anchors.rightMargin
                visible: mouseArea.pressed
                source: "image://theme/meegotouch-list-background-pressed-center"
            }


            Row {
                id: textRow
                anchors {left: toolIcon.right; right: arrow.left; top: parent.top; /*bottom: parent.bottom*/ }
                height: 150

                ToolIcon {
                    id: toolIcon
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    platformIconId: icon
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
                        font.pixelSize: 22
                        color: "#cc6633"

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
                            newModel = xbmc.shares("music");
                            console.log("created model: " + newModel);
                            break;
                        case 3:
                            newModel = xbmc.shares("video");
                            console.log("created model: " + newModel);
                            break;
                        }
                        console.log("setting model: " + newModel);
                        pageStack.push(component, {model: newModel});
                    } else {
                            console.log("Error loading component:", component.errorString());
                    }
                }
            }


            Button {
                id: options
                text: "..."
                width: 80
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                visible: listItem.state != "expanded"
                onClicked: {
                    listItem.state = "expanded"
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
}
