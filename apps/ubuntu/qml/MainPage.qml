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

Page {
    id: mainPage
//    anchors.margins: appWindow.pageMargin
    title: qsTr("XBMC on %1").arg(xbmc.connectedHostName)


    Component.onCompleted: {
        if(settings.musicShowsFiles) {
           mainMenuModel.setProperty(0, "mode", "files");
        }
        if(settings.videoShowsFiles) {
            mainMenuModel.setProperty(1, "mode", "files");
        }
    }

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
        anchors { top: parent.top; left: parent.left; right: parent.right; bottom: parent.bottom }
//        header: headerLayout
        model: mainMenuModel
        spacing: 20
        anchors.margins: pageStack.pageMargin
        property int currentSelected

        delegate:  Item {
            id: listItem
            height: 150
            width: parent.width
            clip: true

            Rectangle {
                id: background
                anchors.fill: parent
                opacity: 0.05
                // Fill page borders
//                anchors.leftMargin: -mainPage.anchors.leftMargin
//                anchors.rightMargin: -mainPage.anchors.rightMargin
                color: "black"
                radius: 20
            }

            BorderImage {
                id: backgroundPressed
                anchors.fill: parent
                // Fill page borders
                visible: mouseArea.pressed
//                source: "image://theme/meegotouch-list-background-pressed-center"
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
//                    source: "image://theme/" + icon + (theme.inverted ? "-inverse" : "")
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
                        text: mode === "library" ? qsTr("Library") : qsTr("Files")
                        font.weight: Font.Light
                        font.pixelSize: 24
                        color: "#848684"
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
}
