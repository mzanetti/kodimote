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
import "../components/"

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
    }

    SilicaFlickable {
        id: flickable
        interactive: !listView.flicking
        anchors.fill: parent

        PullDownMenu {
            MenuPlayerControls {

            }

            MenuItem {
                text: qsTr("Home")
                onClicked: {
                    pageStack.pop(mainPage);
                }
            }
        }


        Column {
            id: headerContainer
            width: parent.width

            PageHeader {
                id: header
                title: model.title
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
            anchors.top: headerContainer.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            clip: true

            property bool useThumbnails: settings.useThumbnails
            property int itemHeight: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 122 : 88

            header: !model.busy && browserPage.model.allowSearch ? searchFieldComponent : null

            delegate: Drawer {
                id: drawer

                width: parent.width
                height: opened && listView.height * drawer._progress > contentItem.height ? listView.height * drawer._progress : contentItem.height
                dock: Dock.Bottom

                background: Item {
                    anchors.fill: parent
                    width: drawer.width - 20

                    Loader {
                        id: contentLoader
                        anchors.fill: parent

                        Connections {
                            target: contentLoader.item

                            onPlayItem: {
                                print("playItem()!")
                                browserPage.model.playItem(filterModel.mapToSourceIndex(index))
                            }

                            onAddToPlaylist: {
                                browserPage.model.addToPlaylist(filterModel.mapToSourceIndex(index))
                            }
                        }
                    }
                }

                backgroundSize: drawer.height - contentItem.height

                ListItem {
                    id: contentItem
                    height: listView.itemHeight
                    contentHeight: listView.itemHeight
                    width: parent.width
                    anchors.topMargin: Theme.paddingSmall
                    anchors.rightMargin: Theme.paddingSmall

                    onPressed: {
                        listView.currentIndex = index
                    }

                    onPressAndHold: {
                        if(browserPage.model.hasDetails()) {
                            browserPage.model.fetchItemDetails(filterModel.mapToSourceIndex(listView.currentIndex));
                            drawer.open = true;
                        }
                    }

                    onClicked: {
                        if (drawer.open) {
                            drawer.open = false;
                        } else {
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
                    }

                    Rectangle {
                        id: highlightBar
                        color: Theme.highlightColor
                        width: 8
                        anchors.top: thumbnailImage.top
                        anchors.bottom: thumbnailImage.bottom
                        anchors.right: thumbnailImage.left
                        anchors.rightMargin: 2
                        visible: playcount === 0
                    }

                    Thumbnail {
                        id: thumbnailImage
                        height: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 120 : (browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatNone ? 0 : 86 )
                        width: browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? 80 : (browserPage.model.thumbnailFormat === XbmcModel.ThumbnailFormatLandscape ? 152 : height)

                        anchors.left: parent.left
                        anchors.leftMargin: Theme.paddingLarge
                        anchors.top: parent.top
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
                        anchors {
                            left: (thumbnailImage.visible ? thumbnailImage.right : parent.left);
                            leftMargin: (thumbnailImage.visible ? Theme.paddingMedium : Theme.paddingLarge);
                            top: parent.top;
                            right: parent.right;
                            rightMargin: Theme.paddingSmall
                        }
                        height: listView.itemHeight

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

                states: [
                    State {
                        when: open
                        PropertyChanges { target: listView; interactive: false;contentY: listView.itemHeight * listView.currentIndex }
                        PropertyChanges { target: flickable; interactive: false }
                        PropertyChanges { target: contentLoader; source: "../components/ItemDetails.qml" }
                        PropertyChanges { target: listView; header: null }
                    },
                    State {
                        when: opened
                        PropertyChanges { target: listView; interactive: false }
                        PropertyChanges { target: flickable; interactive: false }
                    }
                ]
            }

            Behavior on contentY {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 300
                }
            }
        }
    }

    Component {
        id: searchFieldComponent

        SearchField {
            id: searchField
            width: parent.width

            Binding {
                target: filterModel
                property: "filter"
                value: searchField.text
            }
        }
    }
}
