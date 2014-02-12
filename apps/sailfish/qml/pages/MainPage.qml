/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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
import "../components/"

Page {
    id: mainPage

    property bool connected: xbmc.connected

    states: [
        State {
            when: connected
            PropertyChanges { target: listView.headerItem; visible: true }
            PropertyChanges { target: mainMenu; visible: true; enabled: true }
            PropertyChanges { target: listView; model: mainMenuModel }
            PropertyChanges { target: quitXbmc; visible: true }
            PropertyChanges { target: noConnection; visible: false }
        }
    ]

    onConnectedChanged: {
        pageStack.pop(mainPage);
        populateMainMenu();
    }

    function showConnect() {
        var component = Qt.createComponent("ConnectionDialog.qml")
        if (component.status === Component.Ready) {
            component.createObject(mainPage).open()
        } else {
            console.log("Error loading component:", component.errorString());
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: null
        property int currentSelected

        header: PageHeader {
            id: pageHeader
            visible: false
            title: qsTr("XBMC on %1").arg(xbmc.connectedHostName)
        }

        PullDownMenu {
            id: mainMenu
            visible: false
            enabled: false
            MenuPlayerControls {

            }
        }

        PushUpMenu {
            MenuItem {
                text: qsTr("Connect...")
                onClicked: showConnect()
            }

            MenuItem {
                id: quitXbmc
                visible: false
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
                text: qsTr("About")
                onClicked: {
                    var component = Qt.createComponent("AboutDialog.qml")
                    if (component.status === Component.Ready) {
                        component.createObject(mainPage).open()
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }
        }

        NoConnection {
            id: noConnection
            anchors.fill: parent
            visible: true

            onShowConnect: mainPage.showConnect()
        }

        delegate: ListItem {
            id: listItem

            contentHeight: Theme.itemSizeExtraLarge
            menu: contextMenuComponent

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
                    color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
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
                    visible: libraryTarget && sharesTarget
                    color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            onPressed: listView.currentSelected = index;

            showMenuOnPressAndHold: hasMenu

            onClicked: {
                var component = Qt.createComponent("BrowserPage.qml")
                if (component.status === Component.Ready) {
                    var newModel;
                    if (mode === "library") {
                        newModel = xbmc[libraryTarget]();
                    } else {
                        newModel = xbmc.shares(target);
                    }

                    console.log("setting model: " + newModel);
                    pageStack.push(component, {model: newModel});
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }

            Component {
                id: contextMenuComponent

                ContextMenu {
                    id: contextMenu

                    MenuItem {
                        visible: mode !== "files" && mode !== "single"
                        text: qsTr("Show files")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "files";
                            settings[target + "ShowsFiles"] = true;
                        }
                    }
                    MenuItem {
                        visible: mode !== "library" && mode !== "single"
                        text: qsTr("Show library")
                        onClicked: {
                            listView.model.get(listView.currentSelected).mode = "library";
                            settings[target + "ShowsFiles"] = false;
                        }
                    }
                    MenuItem {
                        text: qsTr("Rescan library")
                        onClicked: {
                            var lib = xbmc[libraryTarget]();
                            lib.scanForContent();
                            lib.exit();
                        }
                    }
                    MenuItem {
                        text: qsTr("Clean library")
                        onClicked: {
                            var lib = xbmc[libraryTarget]();
                            lib.clean();
                            lib.exit();
                        }
                    }
                }
            }
        }
    }

    Timer {
        /*Hack, we can't use pageStack.push in the onCompleted handler*/
        id: delayedConnect
        interval: 10
        repeat: false
        running: false
        onTriggered: showConnect()
    }

    Component.onCompleted: {
        if (!xbmc.connected && !xbmc.connecting) {
            delayedConnect.start();
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
            libraryTarget: "audioLibrary"
            sharesTarget: "music"
            hasMenu: true
        }
        ListElement {
            icon: "icon-m-content-videos"
            subtitle: ""
            mode: "library"
            target: "video"
            libraryTarget: "videoLibrary"
            sharesTarget: "video"
            hasMenu: true
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "files"
            target: "pictures"
            sharesTarget: "picture"
            hasMenu: false
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "library"
            target: "tv"
            libraryTarget: "channelGroups"
            hasMenu: false
        }
    }
    ListModel {
        id: mainMenuModel
        // workaround: its not possible to have qsTr() in ListElements for now...
        function title(index) {
            var item = mainMenuModel.get(index);

            if (item) {
                var target = mainMenuModel.get(index).target;
                if (target === "music") {
                    return qsTr("Music");
                }
                if (target === "video") {
                    return qsTr("Videos");
                }
                if (target === "pictures") {
                    return qsTr("Pictures");
                }
                if (target === "tv") {
                    return qsTr("TV Channels");
                }
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

        if(settings.musicShowsFiles) {
           mainMenuModel.setProperty(0, "mode", "files");
        }
        if(settings.videoShowsFiles) {
            mainMenuModel.setProperty(1, "mode", "files");
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
