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
        ListElement {
            title: "Music Files"
            icon: "content-music-band"
            subtitle: ""
        }
        ListElement {
            title: "Video Files"
            icon: "content-videos"
            subtitle: ""
        }
    }


    ListView {
        id: listView
        anchors.fill: parent
//        header: headerLayout
        model: mainMenuModel


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

            ToolIcon {
                id: toolIcon
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                platformIconId: icon
            }

            Row {
                anchors {left: toolIcon.right; right: arrow.left; top: parent.top; bottom: parent.bottom }

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
                        text: subtitle
                        font.weight: Font.Light
                        font.pixelSize: 22
                        color: "#cc6633"

                        visible: text != ""
                    }
                }
            }

            Image {
                id: arrow
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: mouseArea
                anchors.fill: background
                onClicked: {
                    var component = Qt.createComponent("BrowserPage.qml")
                    var newModel;
                    if (component.status == Component.Ready) {
                        switch(index) {
                        case 0:
                            newModel = xbmc.audioLibrary();
                            break
                        case 1:
                            newModel = xbmc.videoLibrary();
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
        }
    }
    ScrollDecorator {
        flickableItem: listView
    }
}
