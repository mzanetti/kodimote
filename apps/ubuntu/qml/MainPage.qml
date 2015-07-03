/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0 as ListItems
import "components"

KodiPage {
    id: mainPage
    //    anchors.margins: appWindow.pageMargin
    title: qsTr("Media Browser")

    property int spacing: units.gu(2)

    Component.onCompleted: {
        populateMainMenu();
    }


    ListModel {
        id: mainMenuModelTemplate
        ListElement {
            icon: "icon-m-content-audio"
            subtitle: ""
            target: "music"
        }
        ListElement {
            icon: "icon-m-content-videos"
            subtitle: ""
            target: "videos"
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
            target: "pictures"
        }
        ListElement {
            icon: "icon-m-content-image"
            subtitle: ""
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
                return qsTr("Live TV");
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
        if (kodi.pvrAvailable && settings.pvrEnabled) {
            mainMenuModel.append(mainMenuModelTemplate.get(3));
        }
    }

    Connections {
        target: kodi
        onPvrAvailableChanged: {
            populateMainMenu();
        }
    }

    Connections {
        target: settings
        onMusicEnabledChanged: populateMainMenu();
        onVideosEnabledChanged: populateMainMenu();
        onPicturesEnabledChanged: populateMainMenu();
        onPvrEnabledChanged: populateMainMenu();
    }

    GridLayout {
        id: grid
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: bottomEdge.top }
        anchors.margins: appWindow.pageMargins
        property int currentSelected
        rowSpacing: mainPage.spacing
        columnSpacing: mainPage.spacing
        columns: width > height ? 2 : 1

        Repeater {
            model: mainMenuModel
            delegate:  UbuntuShape {
                id: listItem
                Layout.fillWidth: true
                Layout.fillHeight: true
                width: parent.width

                //            color: Qt.rgba(0, 0, 0, mouseArea.pressed ? 0.1 : 0.05)
                gradientColor: "black"
                radius: "medium"

                image: Image {
                    source: "images/" + target + ".jpg"
                    fillMode: Image.PreserveAspectCrop
                }

                UbuntuShape {
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
                    highp vec4 sourceColor = texture2D(source, qt_TexCoord0);
                    highp float alpha = 1.0 - qt_TexCoord0.x;
                    sourceColor *= alpha;
                    gl_FragColor = sourceColor;
                }
                "
                }

                Label {
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(4)
                        verticalCenter: parent.verticalCenter
                    }
                    id: mainText
                    text: mainMenuModel.title(index)
                    font.weight: Font.Bold
                    fontSize: "large"
                    color: "white"
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
                                case "rescan":
                                    var lib = kodi.audioLibrary();
                                    if (index == 0) {
                                        lib = kodi.audioLibrary();
                                    } else {
                                        lib = kodi.videoLibrary();
                                    }
                                    lib.scanForContent();
                                    lib.exit();
                                    break;
                                case "clean":
                                    var lib = kodi.audioLibrary();
                                    if (index == 0) {
                                        lib = kodi.audioLibrary();
                                    } else {
                                        lib = kodi.videoLibrary();
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
                                newModel = kodi.audioLibrary();
                                break
                            case "videos":
                                newModel = kodi.videoLibrary();
                                break;
                            case "pictures":
                                newModel = kodi.shares("pictures");
                                console.log("created model: " + newModel);
                                break;
                            case "tv":
                                newModel = kodi.pvrMenu();
                                console.log("created model: " + newModel);
                                break;
                            }
                            console.log("MainPage: setting model: " + newModel);
                            pageStack.push(component, {model: newModel});
                        } else {
                            console.log("Error loading component:", component.errorString());
                        }
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
        if (index < 3) {
            longTapModel.append({modelData: qsTr("Rescan library"), actionId: "rescan"})
            longTapModel.append({modelData: qsTr("Clean library"), actionId: "clean"})
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

    BottomEdge {
        id: bottomEdge

        BottomEdgeButton {
            source: "image://theme/settings"
            text: qsTr("Options")
            Layout.fillWidth: true
            onClicked: {
                PopupUtils.open(Qt.resolvedUrl("SettingsSheet.qml"), mainPage)
            }
        }
        BottomEdgeButton {
            source: "image://theme/info"
            text: qsTr("About")
            Layout.fillWidth: true
            onClicked: {
                PopupUtils.open(Qt.resolvedUrl("components/AboutDialog.qml"), mainPage)
            }
        }
    }
}
