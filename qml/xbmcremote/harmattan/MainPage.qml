import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage
    tools: toolBarEntry
    anchors.margins: appWindow.pageMargin

    ListModel {
        id: mainMenuModel
        ListElement {
            icon: "icon-m-content-audio"
            subtitle: ""
            mode: "library"
        }
        ListElement {
            icon: "icon-m-content-videos"
            subtitle: ""
            mode: "library"
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "files"
        }
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            if (title["text"] === undefined) {
                title.text = [
                    qsTr("Music"),
                    qsTr("Videos"),
                    qsTr("Pictures")
                ]
            }
            return title.text[index];
        }
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
                        text: listView.model.title(index)
                        font.weight: Font.Bold
                        font.pixelSize: 26
                    }

                    Label {
                        id: subText
                        text: mode == "library" ? qsTr("Library") : qsTr("Files")
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
                            if(mode == "library") {
                                newModel = xbmc.audioLibrary();
                            } else {
                                newModel = xbmc.shares("music");
                            }
                            break
                        case 1:
                            if(mode == "library") {
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
                text: qsTr("Show files")
                visible: mainMenuModel.get(listView.currentSelected).mode != "files"
                onClicked: {
                    mainMenuModel.setProperty(listView.currentSelected, "mode", "files");
                }
            }
            MenuItem {
                text: qsTr("Show library")
                visible: mainMenuModel.get(listView.currentSelected).mode != "library"
                onClicked: {
                    mainMenuModel.setProperty(listView.currentSelected, "mode", "library");
                }
            }
            MenuItem {
                text: qsTr("Rescan library")
                onClicked: {
                    print("current selected is" + listView.currentSelected)
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
