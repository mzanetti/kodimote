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
import Xbmc 1.0
import "../"

Page {
    id: browserPage
    property variant model

    Component.onDestruction: {
        model.exit();
    }

    XbmcFilterModel {
        id: filterModel
        model: browserPage.model
        filterCaseSensitivity: Qt.CaseInsensitive

        // When the model is filtered, contentY is messed up sometimes
        // Lets unset and reset the model to keep the searchbar in place
        onFilterChanged: {
            listView.model = undefined
            listView.model = filterModel
        }
    }

    SilicaFlickable {
        PageHeader {
            id: header
            title: model.title
        }
        interactive: !listView.flicking
        anchors.fill: parent

        PullDownMenu {
            MenuPlayerControls {

            }
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: model.busy
            visible: model.busy
            size: BusyIndicatorSize.Large
        }

        SilicaListView {
            id: listView
            highlightFollowsCurrentItem: true
            model: filterModel
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            width: parent.width

            property bool useThumbnails: settings.useThumbnails
            property int itemHeight: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 122 : 88

            delegate: Item {
                height: contentItem.height
                width: parent.width

                BackgroundItem {
                    id: contentItem
                    height: listView.itemHeight
                    width: parent.width

                    onClicked: {
                        if (filetype === "directory") {
                            var component = Qt.createComponent("BrowserPage.qml")
                            if (component.status === Component.Ready) {
                                var newModel = browserPage.model.enterItem(filterModel.mapToSourceIndex(index));
                                newModel.ignoreArticle = settings.ignoreArticle;
                                pageStack.push(component, {model: newModel});
                            } else {
                                console.log("Error loading component:", component.errorString());
                            }
                        } else {
                            browserPage.model.playItem(filterModel.mapToSourceIndex(index));
                        }
                    }

                    Rectangle {
                        id: highlightBar
                        color: Theme.highlightColor
                        height: parent.height - 4
                        anchors.verticalCenter: parent.verticalCenter
                        width: 8
                        anchors.left: parent.left
                        visible: playcount === 0
                    }

                    Thumbnail {
                        id: thumbnailImage
                        height: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 120 : (browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatNone ? 0 : 86 )
                        width: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 80 : (browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatLandscape ? 152 : height)

                        anchors.left: highlightBar.right
                        anchors.leftMargin: 2
                        anchors.top: parent.top
                        anchors.topMargin: 1
                        visible: listView.useThumbnails && browserPage.model.thumbnailFormat !== XbmcModel.ThumbnailFormatNone

                        artworkSource: thumbnail
                        defaultText: title

                        IconButton {
                            id: playingOverlay
                            icon.source: playingState === "playing" ? "image://theme/icon-m-play" : "image://theme/icon-m-pause"
                            visible: playingState === "playing" || playingState == "paused"
                            z: 2
                            anchors.centerIn: thumbnailImage

                            SequentialAnimation on opacity {
                                loops: Animation.Infinite
                                running: playingOverlay.visible
                                NumberAnimation {target: playingOverlay; properties: "opacity"; from: 1; to: 0.5; duration: 1000; easing.type: Easing.InOutQuad}
                                NumberAnimation {target: playingOverlay; properties: "opacity"; from: 0.5; to: 1; duration: 1000; easing.type: Easing.InOutQuad}
                            }
                        }
                    }

                    Row {
                        id: itemRow
                        anchors {left: parent.left; top: parent.top; right: parent.right }
                        height: listView.itemHeight
                        anchors.leftMargin: (settings.useThumbnails ? thumbnailImage.width : 0) + 15
                        anchors.rightMargin: 5

                        Column {
                            anchors.verticalCenter: parent.verticalCenter

                            Text {
                                id: mainText
                                text: title
                                font.weight: Font.Bold
                                font.pixelSize: Theme.fontSizeMedium
                                width: itemRow.width
                                elide: Text.ElideRight
                                color: Theme.primaryColor

                                states: [
                                    State {
                                        name: "highlighted"; when: playingState === "playing" || playingState === "paused"
                                        PropertyChanges { target: mainText; color: Theme.highlightColor }
                                    }

                                ]
                            }

                            Label {
                                id: subText
                                text: subtitle
                                font.weight: Font.Light
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.secondaryColor
                                width: mainText.width
                                elide: Text.ElideRight
                                visible: text != ""
                            }
                            Label {
                                id: subSubText
                                text: year
                                font.weight: Font.Light
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.secondaryColor
                                width: mainText.width
                                elide: Text.ElideRight
                                visible: text != ""
                            }
                        }
                    }
                }
            }
        }
    }
}
