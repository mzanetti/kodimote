/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.kodimote 1.0
import "../components/"

Page {
    id: browserPage
    property variant model
    property bool showSearch: !model.busy && browserPage.model && browserPage.model.allowSearch

    signal home();

    Component.onCompleted: {
        var setting = model.watchedFilterSetting;
        if (setting) {
            settings[setting + 'Changed'].connect(function() {
                filterModel.hideWatched = !settings[setting];
            });
        }
    }

    Component.onDestruction: {
        if (model) {
            model.exit();
        }
    }

    KodiFilterModel {
        id: filterModel
        model: browserPage.model
        filterCaseSensitivity: Qt.CaseInsensitive
        hideWatched: model.watchedFilterSetting ? !settings[model.watchedFilterSetting] : false

        onHideWatchedChanged: {
            var setting = model.watchedFilterSetting;
            if (setting) {
                settings[setting] = !hideWatched;
            }
        }
    }

    SilicaFlickable {
        id: flickable
        interactive: !listView.flicking
        pressDelay: 0
        anchors.fill: parent

        PullDownMenu {
            ControlsMenuItem {

            }

            MenuItem {
                text: qsTr("Home")
                onClicked: {
                    browserPage.home();
                }
            }

            MenuItem {
                text: qsTr("Keypad")
                onClicked: {
                    pageStack.push("Keypad.qml")
                }
            }

            MenuItem {
                text: qsTr("Now Playing")
                enabled: kodi.activePlayer !== null
                onClicked: {
                    pageStack.push("NowPlayingPage.qml")
                }
            }
        }

        PageHeader {
            id: header
            title: model ? model.title : ""
            width: parent.width
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: model && model.busy
            visible: model && model.busy
            size: BusyIndicatorSize.Large
        }

        SilicaListView {
            id: listView
            highlightFollowsCurrentItem: true
            model: filterModel
            anchors.top: header.bottom
            anchors.bottom: controlBar.top
            width: parent.width
            clip: true
            pressDelay: 0
            cacheBuffer: itemHeight * 3

            property bool useThumbnails: settings.useThumbnails
            property int itemHeight: browserPage.model && browserPage.model.thumbnailFormat === KodiModel.ThumbnailFormatPortrait ? 126 : 92

            delegate: Drawer {
                id: drawer

                width: parent.width
                height: opened && listView.height * drawer._progress > contentItem.height ? listView.height * drawer._progress : contentItem.height
                dock: Dock.Bottom

                function playItem() {
                    if (resume === 0) {
                        browserPage.model.playItem(filterModel.mapToSourceIndex(index));
                    } else {
                        var dialog = pageStack.push("ResumeDialog.qml", {item: model});
                        dialog.onAccepted.connect(function() {
                            browserPage.model.playItem(filterModel.mapToSourceIndex(index), true);
                        });
                        dialog.onRejected.connect(function() {
                            browserPage.model.playItem(filterModel.mapToSourceIndex(index));
                        });
                    }
                }

                background: Item {
                    anchors.fill: parent
                    width: drawer.width - 20

                    Loader {
                        id: contentLoader
                        anchors.fill: parent

                        onLoaded: {
                            item.item = browserPage.model.getItem(index);
                        }

                        Connections {
                            target: contentLoader.item

                            onPlayItem: {
                                drawer.playItem();
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
                    contentHeight: listView.itemHeight
                    width: parent.width
                    anchors.topMargin: Theme.paddingSmall
                    anchors.rightMargin: Theme.paddingSmall
                    showMenuOnPressAndHold: playable && !browserPage.model.hasDetails()

                    onPressed: {
                        listView.currentIndex = index
                    }

                    onPressAndHold: {
                        if (browserPage.model.hasDetails()) {
                            browserPage.model.fetchItemDetails(filterModel.mapToSourceIndex(listView.currentIndex));
                            drawer.open = true;
                        }
                    }

                    onClicked: {
                        if (drawer.open) {
                            drawer.open = false;
                        } else {
                            if (filetype === "directory") {
                                var newModel = browserPage.model.enterItem(filterModel.mapToSourceIndex(index));
                                newModel.ignoreArticle = settings.ignoreArticle;
                                var browser = pageStack.push("BrowserPage.qml", {model: newModel});
                                browser.home.connect(function() {
                                    browserPage.home();
                                });
                            } else {
                                drawer.playItem();
                            }
                        }
                    }

                    Rectangle {
                        id: highlightBar
                        color: resume <= 0 ? Theme.highlightColor : Theme.secondaryHighlightColor
                        width: 8
                        anchors.top: thumbnailImage.top
                        anchors.bottom: thumbnailImage.bottom
                        anchors.right: thumbnailImage.left
                        anchors.rightMargin: 2
                        visible: playcount === 0 || resume > 0
                    }

                    menu: Component {
                        ContextMenu {
                            MenuItem {
                                text: qsTr("Play")
                                onClicked: drawer.playItem()
                            }
                            MenuItem {
                                text: qsTr("Add to playlist")
                                onClicked: browserPage.model.addToPlaylist(filterModel.mapToSourceIndex(index))
                            }
                        }
                    }

                    Thumbnail {
                        id: thumbnailImage
                        height: browserPage.model.thumbnailFormat === KodiModel.ThumbnailFormatPortrait ? 120 : (browserPage.model.thumbnailFormat === KodiModel.ThumbnailFormatNone ? 0 : 86 )
                        width: browserPage.model.thumbnailFormat === KodiModel.ThumbnailFormatPortrait ? 80 : (browserPage.model.thumbnailFormat === KodiModel.ThumbnailFormatLandscape ? 152 : height)

                        anchors.left: parent.left
                        anchors.leftMargin: Theme.paddingLarge
                        anchors.verticalCenter: parent.verticalCenter
                        visible: listView.useThumbnails && browserPage.model.thumbnailFormat !== KodiModel.ThumbnailFormatNone

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
                            rightMargin: Theme.paddingLarge
                        }
                        height: listView.itemHeight

                        Column {
                            anchors.verticalCenter: parent.verticalCenter

                            Label {
                                id: mainText
                                text: title
                                font.weight: Font.Bold
                                font.pixelSize: Theme.fontSizeMedium
                                width: itemRow.width
                                truncationMode: TruncationMode.Fade
                                color: Theme.primaryColor
                                height: font.pixelSize
                                verticalAlignment: Text.AlignVCenter

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
                                truncationMode: TruncationMode.Fade
                                visible: text != ""
                                height: font.pixelSize
                                verticalAlignment: Text.AlignVCenter
                            }

                            Label {
                                id: subSubText
                                text: year
                                font.weight: Font.Light
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.secondaryColor
                                width: mainText.width
                                truncationMode: TruncationMode.Fade
                                visible: text != ""
                                height: font.pixelSize
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    ProgressBar {
                        minimumValue: 0
                        maximumValue: 100
                        value: model.progressPercentage != undefined ? progressPercentage : 0
                        visible: index >= 0 && browserPage.model.getItem(filterModel.mapToSourceIndex(index)).type === "channel"

                        leftMargin: 10
                        rightMargin: 10
                        height: 20
                        anchors {
                            left: (thumbnailImage.visible ? thumbnailImage.right : parent.left);
                            leftMargin: (thumbnailImage.visible ? Theme.paddingSmall : Theme.paddingLarge);
                            right: parent.right
                            rightMargin: Theme.paddingLarge
                            verticalCenter: parent.bottom
                            verticalCenterOffset: 25
                        }
                    }

                }

                states: [
                    State {
                        when: open
                        PropertyChanges { target: listView; interactive: false; contentY: listView.itemHeight * listView.currentIndex }
                        PropertyChanges { target: flickable; interactive: false }
                        PropertyChanges { target: contentLoader; source: browserPage.model.getItem(filterModel.mapToSourceIndex(index)).type == "channel" ? "../components/ChannelDetails.qml" : "../components/ItemDetails.qml" }
                        PropertyChanges { target: dockedControls; hideTemporary: true }
                        PropertyChanges { target: controlBar; height: 0 }
                    },
                    State {
                        when: opened
                        PropertyChanges { target: listView; interactive: false }
                        PropertyChanges { target: flickable; interactive: false }
                        PropertyChanges { target: dockedControls; hideTemporary: true }
                    }
                ]
            }

            Behavior on contentY {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 300
                }
            }

            VerticalScrollDecorator { }
        }

        Column {
            id: controlBar
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingLarge
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingSmall

            SearchField {
                id: searchField
                width: parent.width

                visible: browserPage.showSearch && searchSwitch.checked

                Binding {
                    target: filterModel
                    property: "filter"
                    value: searchField.text
                }
            }

            Row {
                Switch {
                    id: searchSwitch
                    icon.source: "image://theme/icon-m-search"
                    icon.height: 32
                    icon.width: 32
                    visible: browserPage.showSearch
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -14
                    onCheckedChanged: {
                        if (searchSwitch.checked) {
                            searchField.forceActiveFocus();
                        }
                    }
                }

                Switch {
                    icon.source: "image://theme/icon-s-installed"
                    visible: model.allowWatchedFilter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -14
                    checked: !filterModel.hideWatched
                    automaticCheck: false
                    onClicked: {
                        filterModel.hideWatched = !filterModel.hideWatched
                    }
                }

                IconButton {
                    icon.source: "image://theme/icon-direction-forward"
                    rotation: filterModel.sortOrder == Qt.AscendingOrder ? 180 : 0
                    height: 70
                    width: 70
                    icon.height: 70
                    icon.width: 70
                    onClicked: {
                        filterModel.sortOrder = filterModel.sortOrder == Qt.AscendingOrder ? Qt.DescendingOrder : Qt.AscendingOrder
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Behavior on height {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 300
                }
            }
        }
    }
}
