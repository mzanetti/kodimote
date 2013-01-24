/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage
    tools: toolBarEntry
//    anchors.margins: appWindow.pageMargin

    Component.onCompleted: {
        if(settings.musicShowsFiles) {
           mainMenuModel.setProperty(0, "mode", "files");
        }
        if(settings.videoShowsFiles) {
            mainMenuModel.setProperty(1, "mode", "files");
        }
    }

    Image {
        id: listHeader
        anchors {left: parent.left; top: parent.top; right: parent.right }
        source: "image://theme/meegotouch-view-header-fixed" + (theme.inverted ? "-inverted" : "")
        z: listView.z + 1
        Label {
            anchors.margins: 10
            anchors.fill: parent
            anchors.verticalCenter: listHeader.verticalCenter
            font.pixelSize: 28
            text: qsTr("XBMC on %1").arg(xbmc.connectedHostName)
            elide: Text.ElideLeft
        }
    }

    ListModel {
        id: mainMenuModel
        ListElement {
            icon: "image://theme/icon-m-content-audio"
            inverseIcon: "image://theme/icon-m-content-audio-inverse"
            subtitle: ""
            mode: "library"
        }
        ListElement {
            icon: "image://theme/icon-m-content-videos"
            inverseIcon: "image://theme/icon-m-content-videos-inverse"
            subtitle: ""
            mode: "library"
        }
        ListElement {
            icon: "image://theme/icon-m-content-image"
            inverseIcon: "image://theme/icon-m-content-image-inverse"
            subtitle: ""
            mode: "single"
        }
        ListElement {
            icon: "image://theme/icon-m-content-tv-show"
            inverseIcon: "icons/icon-m-content-tv-show-inverse.png"
            subtitle: ""
            mode: "single"
        }
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            if (title["text"] === undefined) {
                title.text = [
                    qsTr("Music"),
                    qsTr("Videos"),
                    qsTr("Pictures"),
                    qsTr("TV Channels")
                ]
            }
            return title.text[index];
        }
    }


    ListView {
        id: listView
        anchors { top: listHeader.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
//        header: headerLayout
        model: mainMenuModel
        spacing: 20
        anchors.margins: appWindow.pageMargin
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
                    source: (theme.inverted ? inverseIcon : icon)
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
                        text: {
                            if (mode === "library") {
                                return qsTr("Library");
                            } else if (mode == "files") {
                                return qsTr("Files");
                            }
                        }
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
                    if(index === 0 || index  === 1) {
                        longTapMenu.open();
                    }
                }

                onClicked: {
                    var component = Qt.createComponent("BrowserPage.qml")
                    var newModel;
                    if (component.status === Component.Ready) {
                        switch(index) {
                        case 0:
                            if(mode === "library") {
                                newModel = xbmc.audioLibrary();
                            } else {
                                newModel = xbmc.shares("music");
                            }
                            break
                        case 1:
                            if(mode === "library") {
                                newModel = xbmc.videoLibrary();
                            } else {
                                newModel = xbmc.shares("video");
                            }
                            break;
                        case 2:
                            newModel = xbmc.shares("pictures");
                            console.log("created model: " + newModel);
                            break;
                        case 3:
                            newModel = xbmc.channelGroups();
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


    ContextMenu {
        id: longTapMenu
        visualParent: pageStack

        MenuLayout {
            MenuItem {
                text: qsTr("Show files")
                visible: mainMenuModel.get(listView.currentSelected).mode !== "files"
                onClicked: {
                    mainMenuModel.setProperty(listView.currentSelected, "mode", "files");
                    if(listView.currentSelected == 0) {
                        settings.musicShowsFiles = true;
                    }
                    if(listView.currentSelected == 1) {
                        settings.videoShowsFiles = true;
                    }
                }
            }
            MenuItem {
                text: qsTr("Show library")
                visible: mainMenuModel.get(listView.currentSelected).mode !== "library"
                onClicked: {
                    mainMenuModel.setProperty(listView.currentSelected, "mode", "library");
                    if(listView.currentSelected == 0) {
                        settings.musicShowsFiles = false;
                    }
                    if(listView.currentSelected == 1) {
                        settings.videoShowsFiles = false;
                    }
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
            MenuItem {
                text: qsTr("Clean library")
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

                    lib.clean();
                    lib.exit();
                }
            }
        }
    }
}
