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

import QtQuick 2.0
import Sailfish.Silica 1.0
import ".."

Page {
    id: mainPage

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuPlayerControls {

            }

            MenuItem {
                text: qsTr("Quit xbmc")
                onClicked: {
                    var component = Qt.createComponent("QuitDialog.qml")
                    if (component.status === Component.Ready) {
                        component.createObject(mainPage).open()
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }

            MenuItem {
                text: qsTr("Connect...")
                onClicked: {
                    var component = Qt.createComponent("ConnectionDialog.qml")
                    if (component.status === Component.Ready) {
                        component.createObject(mainPage).open()
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }
        }

        PageHeader {
            id: header
            title: qsTr("XBMC on %1").arg(xbmc.connectedHostName)
        }

        SilicaListView {
            id: listView
            anchors { top: header.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
            model: mainMenuModel
            interactive: false
            property Item contextMenu
            property int currentSelected

            delegate: Item {
                id: listItem
                property bool menuOpen: listView.contextMenu != null && listView.contextMenu.parent === listItem

                height: menuOpen ? listView.contextMenu.height + contentItem.height : contentItem.height
                width: parent.width
                BackgroundItem {
                    id: contentItem
                    height: Theme.itemSizeExtraLarge
                    width: parent.width

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: 14

                        Label {
                            id: mainText
                            text: listView.model.title(index)
                            font.weight: Font.Bold
                            font.pixelSize: Theme.fontSizeLarge
                            color: Theme.primaryColor
                        }

                        Label {
                            id: subText
                            text: {
                                if (mode === "library") {
                                    return qsTr("Library");
                                } else if (mode == "files") {
                                    return qsTr("Files");
                                } else {
                                    return ""
                                }
                            }
                            visible: text != ""
                            color: Theme.secondaryColor
                        }
                    }

                    onPressed: listView.currentSelected = index;

                    onPressAndHold: {
                        if (!listView.model.get(index).hasMenu)
                            return;

                        if (!listView.contextMenu)
                            listView.contextMenu = contextMenuComponent.createObject(listView);
                        listView.contextMenu.show(listItem);
                    }

                    onClicked: {
                        var component = Qt.createComponent("BrowserPage.qml")
                        var newModel;
                        if (component.status === Component.Ready) {
                            if (target === "music") {
                                if(mode === "library") {
                                    newModel = xbmc.audioLibrary();
                                } else {
                                    newModel = xbmc.shares("music");
                                }
                            } else if (target === "videos") {
                                if(mode === "library") {
                                    newModel = xbmc.videoLibrary();
                                } else {
                                    newModel = xbmc.shares("video");
                                }
                            } else if (target === "pictures") {
                                newModel = xbmc.shares("pictures");
                                console.log("created model: " + newModel);
                            } else if (target === "pvr") {
                                newModel = xbmc.channelGroups();
                                console.log("created model: " + newModel);
                            }
                            console.log("setting model: " + newModel);
                            pageStack.push(component, {model: newModel});
                        } else {
                            console.log("Error loading component:", component.errorString());
                        }
                    }
                }
            }

            Component {
                id: contextMenuComponent

                ContextMenu {
                    id: contextMenu
                    property string mode: listView.model.get(listView.currentSelected).mode
                    property string target: listView.model.get(listView.currentSelected).target
                    property string targetNormalized: {
                        if (target === "videos") {
                            return "video";
                        } else if (target === "pictures") {
                            return "picture";
                        } else {
                            return target;
                        }
                    }

                    MenuItem {
                        visible: mode !== "files" && mode !== "single"
                        text: qsTr("Show files")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "files";
                            settings[contextMenu.targetNormalized + "ShowsFiles"] = true;
                        }
                    }
                    MenuItem {
                        visible: contextMenu.mode !== "library" && contextMenu.mode !== "single"
                        text: qsTr("Show library")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "library";
                            settings[contextMenu.targetNormalized + "ShowsFiles"] = false;
                        }
                    }
                    MenuItem {
                        text: qsTr("Rescan library")
                        onClicked: {
                            var lib = xbmc[contextMenu.targetNormalized + "Library"]();
                            lib.scanForContent();
                            lib.exit();
                        }
                    }
                    MenuItem {
                        text: qsTr("Clean library")
                        onClicked: {
                            var lib = xbmc[contextMenu.targetNormalized + "Library"]();
                            lib.clean();
                            lib.exit();
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        if(settings.musicShowsFiles) {
           mainMenuModel.setProperty(0, "mode", "files");
        }
        if(settings.videoShowsFiles) {
            mainMenuModel.setProperty(1, "mode", "files");
        }
        populateMainMenu();
    }

    ListModel {
        id: mainMenuModelTemplate
        ListElement {
            icon: "icon-m-content-audio"
            subtitle: ""
            mode: "library"
            target: "music"
            hasMenu: true
        }
        ListElement {
            icon: "icon-m-content-videos"
            subtitle: ""
            mode: "library"
            target: "videos"
            hasMenu: true
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "single"
            target: "pictures"
            hasMenu: false
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "single"
            target: "tv"
            hasMenu: false
        }
    }
    ListModel {
        id: mainMenuModel
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            var target = mainMenuModel.get(index).target;
            if (target === "music") {
                return qsTr("Music");
            }
            if (target === "videos") {
                return qsTr("Videos");
            }
            if (target === "pictures") {
                return qsTr("Pictures");
            }
            if (target === "tv") {
                return qsTr("TV Channels");
            }
            return "";
        }
    }

    function populateMainMenu() {
        mainMenuModel.clear();
        if (settings.musicEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(0));
        }
        if (settings.videosEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(1));
        }
        if (settings.picturesEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(2));
        }
        if (settings.pvrEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(3));
        }
    }

    Connections {
        target: settings
        onMusicEnabledChanged: populateMainMenu();
        onVideosEnabledChanged: populateMainMenu();
        onPicturesEnabledChanged: populateMainMenu();
        onPvrEnabledChanged: populateMainMenu();
    }
}
