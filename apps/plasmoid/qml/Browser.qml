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
import org.kde.plasma.components 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import Xbmc 1.0
import Separator 0.1

Row {
    id: root

    property variant model: xbmc.audioLibrary()

    function clear() {
        while(model.parentModel() !== null) {
            model = model.exit();
        }
    }

    Column {
        spacing: root.spacing

        MediaControlButton {
            source: "audio-headphones"
            onClicked: {
                root.clear();
                root.model = xbmc.audioLibrary();
            }

        }
        MediaControlButton {
            source: "media-optical-blu-ray"
            onClicked: {
                root.clear();
                root.model = xbmc.videoLibrary();
            }
        }
        MediaControlButton {
            source: "camera-photo"
            onClicked: {
                root.clear();
                root.model = xbmc.shares("pictures")
            }
        }
        MediaControlButton {
            source: "video-television"
            onClicked: {
                root.clear();
                root.model = xbmc.channelGroups();
            }
        }
        MediaControlButton {
            source: "document-open-folder"
            onClicked: {
                root.clear();
                root.model = xbmc.shares("");
            }
        }
    }

    Separator {
        orientation: Qt.Vertical
        height: parent.height
    }

    Item {
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: (parent.width - x) - column3.width - parent.spacing * 2 - separator.width

        Row {
            id: header
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            spacing: root.spacing / 2

            MediaControlButton {
                source: "go-previous"
                enabled: root.model.parentModel() !== null
                width: theme.iconSizes.small
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if(root.model.parentModel() !== null) {
                        root.model = root.model.exit()
                    }
                }
            }
            Label {
                text: root.model.title
                font.weight: Font.Bold
                width: parent.width - x - filterButton.width - parent.spacing
            }
            MediaControlButton {
                id: filterButton
                source: "view-filter"
                width: theme.iconSizes.small
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    filterTextField.searchEnabled = true;
                    filterTextField.forceActiveFocus();
                }
            }
        }

        XbmcFilterModel {
            id: filterModel
            model: root.model
            caseSensitive: false

            // When the model is filtered, contentY is messed up sometimes
            // Lets unset and reset the model to keep the searchbar in place
            onFilterChanged: {
                list.model = undefined
                list.model = filterModel
            }
        }

        ListView {
            id: list
            clip: true
            currentIndex: 0
            model: filterModel
            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                bottom: filterTextField.top
            }

            function enterItem(index) {
                print("entering item", index)
                var newModel = root.model.enterItem(filterModel.mapToSourceIndex(index));
                newModel.ignoreArticle = settings.ignoreArticle;
                console.log("newModel: " + newModel);
                root.model = newModel;
            }
            function playItem(index) {
                list.model.playItem(index);
            }

            delegate: Item {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                height: titleLabel.height
    //            height: 64
    //            Image {
    //                anchors.fill: parent
    //                source: index === list.currentIndex ? "images/MenuItemFO.png" : "images/MenuItemNF.png"
    //            }

                Image {
                    id: thumbnailImage
                    height: parent.height - 2
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.left: parent.left
                    anchors.leftMargin: root.spacing / 2

        //                width: height
                    fillMode: Image.PreserveAspectFit
                    smooth: false
                    source: settings.useThumbnails ? thumbnail : ""
                    sourceSize.height: parent.height - 2
                    visible: settings.useThumbnails
                }


                Label {
                    id: titleLabel
                    text: title
    //                font.pixelSize: 28
                    anchors {left: parent.left; top: parent.top; bottom: parent.bottom; right: watchedImage.left }
                    anchors.leftMargin: (thumbnailImage.width > 0 ? thumbnailImage.width + root.spacing / 2 : 0) + root.spacing / 2
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                Image {
                    id: watchedImage
                    source: "images/OverlayWatched.png"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    visible: playcount > 0
                }

                ButtonShadow {
                    anchors.fill: parent
                    state: mouseArea.containsMouse ? "hover" : "hidden"
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: list.currentIndex = index

                    onClicked: {
                        if(filetype === "directory") {
                            list.enterItem(index);
                        } else {
                            root.model.playItem(filterModel.mapToSourceIndex(index));
                        }
                    }
                }
            }


            Keys.onPressed: {
                console.log("key pressed");
            }
        }

        TextField {
            id: filterTextField
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: searchEnabled ? 0 : -height
            }
            Behavior on anchors.bottomMargin {
                NumberAnimation { duration: 200 }
            }

            onTextChanged: filterModel.filter = text
            clearButtonShown: text.length > 0
            errorHighlight: text.length > 0 && filterModel.count == 0

            property bool searchEnabled: false
            Timer {
                interval: 5000
                running: filterTextField.searchEnabled && filterTextField.text.length == 0
                repeat: true

                onTriggered: filterTextField.searchEnabled = false;
            }


            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: root.spacing / 2
                text: "Filter..."
                visible: parent.text.length == 0
                opacity: .5
                color: "black"
            }
        }
    }

    Separator {
        id: separator
        orientation: Qt.Vertical
        height: parent.height
    }

    Column {
        id: column3
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: height
        height: parent.height
        spacing: root.spacing

        Component {
            id: libraryButtons
            Column {
                spacing: root.spacing
                Button {
                    text: qsTr("Update library")
                    width: parent.width
                }
                Button {
                    text: qsTr("Clean library")
                    width: parent.width
                }
            }
        }
        Component {
            id: itemDetails
            Image {
                id: thumbnailImage
                fillMode: Image.PreserveAspectFit
                smooth: false
                source: root.model.get(filterModel.mapToSourceIndex(list.currentIndex), "thumbnail")
                sourceSize.height: height

                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        margins: root.spacing / 2
                    }
                    radius: root.spacing / 2
                    color: Qt.rgba(0, 0, 0, 0.6)
                    height: childrenRect.height

                    Column {
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                        }
                        height: childrenRect.height

                        Label {
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                            }
                            text: root.model.get(filterModel.mapToSourceIndex(list.currentIndex), "title")
                        }
                        Label {
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                            }
                            text: root.model.get(filterModel.mapToSourceIndex(list.currentIndex), "subtitle")
                        }
                    }
                }
            }

        }

        Loader {
            anchors.fill: parent
            sourceComponent: root.model.parentModel() == null ? libraryButtons : itemDetails
        }

//        Item {
//            height: parent.height - y - filterTextField.height - parent.spacing
//            width: parent.width
//        }

    }
}

