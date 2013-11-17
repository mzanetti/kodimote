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

Item {
    id: root

    property variant model
    property int currentIndex: (filterModel.model && filterModel.model.count > 0) ? filterModel.mapToSourceIndex(listView.currentIndex) : -1
    property int spacing

    Component.onDestruction: {
        model.exit();
    }

    Item {
        id: header
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: childrenRect.height

        MediaControlButton {
            id: backButton
            source: "go-previous"
            enabled: root.model ? root.model.parentModel() !== null : false
            width: theme.iconSizes.small
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            onClicked: {
                pagestack.pop()
                root.destroy(); // Seems pages are not deleted on pop()... huh?
            }
        }
        Label {
            text: root.model ? root.model.title : ""
            font.weight: Font.Bold
            anchors {
                left: backButton.right
                right: busyIndicator.left
                margins: root.spacing / 2
                verticalCenter: parent.verticalCenter
            }
            elide: Text.ElideRight
        }

        BusyIndicator {
            id: busyIndicator
            anchors {
                right: parent.right
                margins: filterButton.width + root.spacing / 2
                verticalCenter: parent.verticalCenter
            }
            running: root.model ? root.model.busy : false
            visible: running
        }

        MediaControlButton {
            id: filterButton
            source: "view-filter"
            width: theme.iconSizes.small
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            onClicked: {
                filterTextField.searchEnabled = true;
                filterTextField.forceActiveFocus();
            }
        }
    }

    XbmcFilterModel {
        id: filterModel
        model: root.model ? root.model : null
        filterCaseSensitivity: false

        // When the model is filtered, contentY is messed up sometimes
        // Lets unset and reset the model to keep the searchbar in place
        onFilterChanged: {
            listView.model = undefined
            listView.model = filterModel
        }
    }

    ListView {
        id: listView
        clip: true
        currentIndex: 0
        model: filterModel
        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: filterTextField.top
        }

        // No clue why the binding on root.currentIndex doesn't work... setting it manually here
        onCurrentIndexChanged: {
            root.currentIndex = (filterModel.model && filterModel.model.count > 0) ? filterModel.mapToSourceIndex(listView.currentIndex) : -1;
        }

        function enterItem(index) {
            print("entering item", index)
            var newModel = root.model.enterItem(filterModel.mapToSourceIndex(index));
            newModel.ignoreArticle = settings.ignoreArticle;
            console.log("newModel: " + newModel);
            root.model = newModel;
        }
        function playItem(index) {
            listView.model.playItem(index);
        }

        delegate: Item {
            height: titleLabel.height
            width: listView.width - (listView.contentHeight > listView.height ? scrollBar.width + root.spacing / 2 : 0)
            Row {
                anchors.fill: parent
                anchors.leftMargin: spacing
                spacing: root.spacing / 2
                Image {
                    source: thumbnail
                    height: parent.height
                    width: {
                        if (root.model.thumbnailFormat === XbmcModel.ThumbnailFormatNone) {
                            return 0;
                        } else if (root.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait) {
                            return height * 3 / 4;
                        } else if (root.model.thumbnailFormat === XbmcModel.ThumbnailFormatLandscape) {
                            return height * 16 / 9;
                        }
                        return height;
                    }
                }

                Label {
                    id: titleLabel
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - x - watchedImage.width - parent.spacing*2
                    text: title
                    elide: Text.ElideRight
                }
                PlasmaCore.IconItem {
                    id: watchedImage
                    source: "dialog-ok"
                    anchors.verticalCenter: parent.verticalCenter

                    width: theme.iconSizes.small
                    height: width

                    visible: playcount > 0
                }

            }

            ButtonShadow {
                anchors.fill: parent
                state: mouseArea.containsMouse ? "hover" : "hidden"
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    listView.currentIndex = index
                }

                onClicked: {
                    if(filetype === "directory") {
                        var component = Qt.createComponent("BrowserPage.qml");
                        var obj = component.createObject(parent);
                        var newModel = root.model.enterItem(filterModel.mapToSourceIndex(index));
                        newModel.ignoreArticle = settings.ignoreArticle;
                        obj.model = newModel;
                        obj.spacing = root.spacing;
                        pagestack.push(obj)
                    } else {
                        root.model.playItem(filterModel.mapToSourceIndex(index));
                    }
                }
            }
        }

        ScrollBar {
            id: scrollBar
            orientation: Qt.Vertical
            flickableItem: listView
            //stepSize: 40
            scrollButtonInterval: header.height
            anchors {
                top: listView.top
                right: listView.right
                bottom: listView.bottom
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
        enabled: searchEnabled
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
