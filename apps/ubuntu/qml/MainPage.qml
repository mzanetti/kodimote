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
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import "components/ToolbarButtons" as ToolbarButtons

Page {
    id: mainPage
//    anchors.margins: appWindow.pageMargin
    title: qsTr("XBMC on %1").arg(xbmc.connectedHostName)

    property int spacing

    tools: showToolbars ? mainTools : null
    ToolbarItems {
        id: mainTools
        ToolbarButtons.SystemMenu {}
        ToolbarButtons.Spacer {}
        ToolbarButton {
            text: qsTr("Browsing")
            iconSource: "image://theme/settings"
            onTriggered: {
                PopupUtils.open(Qt.resolvedUrl("SettingsSheet.qml"), mainTools)
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
        }
        ListElement {
            icon: "icon-m-content-videos"
            subtitle: ""
            mode: "library"
            target: "videos"
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "files"
            target: "pictures"
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            mode: "library"
            target: "tv"
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

    ListView {
        id: listView
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom }
//        header: headerLayout
        model: mainMenuModel
        spacing: units.gu(2)
        anchors.margins: appWindow.pageMargins
        interactive: false
        property int currentSelected

        delegate:  UbuntuShape {
            id: listItem
            height: (listView.height) / 4 - listView.spacing * 2
            width: parent.width

//            color: Qt.rgba(0, 0, 0, mouseArea.pressed ? 0.1 : 0.05)
            gradientColor: "black"
            radius: "medium"

/*            image: Image {
                source: "images/" + target + ".jpg"
                fillMode: Image.PreserveAspectCrop
            }
*/            UbuntuShape {
                id: sourceShape
                anchors.fill: parent
                color: "black"
                radius: "medium"
            }

            ShaderEffect {
                anchors.fill: parent
                property variant source: ShaderEffectSource {
                    sourceItem: sourceShape
                    hideSource: true
                }

                fragmentShader: "
                varying highp vec2 qt_TexCoord0;
                uniform sampler2D source;
                void main(void)
                {
                    vec4 sourceColor = texture2D(source, qt_TexCoord0);
                    float alpha = 1.0 - qt_TexCoord0.x;
                    sourceColor *= alpha;
                    gl_FragColor = sourceColor;
                }
                "
            }

            Row {
                id: textRow
                anchors.fill: parent
                height: 150
                spacing: units.gu(4)
                anchors.margins: units.gu(4)


                Image {
                    id: toolIcon
                    anchors.verticalCenter: parent.verticalCenter
//                    source: "image://theme/" + icon + (theme.inverted ? "-inverse" : "")
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: listView.model.title(index)
                        font.weight: Font.Bold
                        fontSize: "large"
                        color: "white"
                    }

                    Label {
                        id: subText
                        text: mode === "library" ? qsTr("Library") : qsTr("Files")
                        visible: text != ""
                        color: "white"
                    }
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onPressed: listView.currentSelected = index;

                onPressAndHold: {

                    if (mainMenuModel.get(index).target === "music" || mainMenuModel.get(index).target === "videos") {
                        var popover = openLongTapMenu(listItem, index)
                        popover.selected.connect(function(actionId) {
                            switch (actionId) {
                            case 0:
                                mainMenuModel.get(index).mode = "files"
                                if (mainMenuModel.get(index).target === "music") {
                                    settings.musicShowsFiles = true;
                                } else if (mainMenuModel.get(index).target === "videos") {
                                    settings.videosShowsFiles = true;
                                }
                                break;
                            case 1:
                                mainMenuModel.get(index).mode = "library"
                                if (mainMenuModel.get(index).target === "music") {
                                    settings.musicShowsFiles = false;
                                } else if (mainMenuModel.get(index).target === "videos") {
                                    settings.videosShowsFiles = false;
                                }
                            case 2:
                                var lib = xbmc.audioLibrary();
                                if (index == 0) {
                                    lib = xbmc.audioLibrary();
                                } else {
                                    lib = xbmc.videoLibrary();
                                }
                                lib.scanForContent();
                                lib.exit();
                                break;
                            case 3:
                                var lib = xbmc.audioLibrary();
                                if (index == 0) {
                                    lib = xbmc.audioLibrary();
                                } else {
                                    lib = xbmc.videoLibrary();
                                }

                                lib.clean();
                                lib.exit();
                            }
                            PopupUtils.close(popover)
                        })
                    }
                }

                onClicked: {
                    var component = Qt.createComponent("BrowserPage.qml")
                    var newModel;
                    if (component.status === Component.Ready) {
                        switch(mainMenuModel.get(index).target) {
                        case "music":
                            if(mode === "library") {
                                newModel = xbmc.audioLibrary();
                            } else {
                                newModel = xbmc.shares("music");
                            }
                            break
                        case "videos":
                            if(mode === "library") {
                                newModel = xbmc.videoLibrary();
                            } else {
                                newModel = xbmc.shares("video");
                            }
                            break;
                        case "pictures":
                            newModel = xbmc.shares("pictures");
                            console.log("created model: " + newModel);
                            break;
                        case "tv":
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

    ListModel {
        id: longTapModel
    }

    function openLongTapMenu(parent, index) {
        print("opening longtap for index", index)
        longTapModel.clear()
        if (mainMenuModel.get(index).mode == "library") {
            longTapModel.append({modelData: qsTr("Show files"), actionId: 0})
        }
        if (mainMenuModel.get(index).mode == "files") {
            longTapModel.append({modelData: qsTr("Show library"), actionId: 1})
        }
        if (index < 3) {
            longTapModel.append({modelData: qsTr("Rescan library"), actionId: 2})
            longTapModel.append({modelData: qsTr("Clean library"), actionId: 3})
        }
        return PopupUtils.open(longTapMenu, parent, {model: longTapModel})
    }

    Component {
        id: longTapMenu
        Popover {
            id: popover
            property alias model: listView.model
            signal selected(int actionId)
            ListView {
                id: listView
                width: parent.width
                height: contentHeight
                delegate: ListItems.Standard {
                    text: modelData
                    onClicked: popover.selected(actionId)
                }
            }
        }
    }
}
