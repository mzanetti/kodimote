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
import Ubuntu.Components.ListItems 0.1 as ListItems
import Xbmc 1.0

Page {
    id: root
    title: model.title

    property variant model
    property int spacing: units.gu(1)

    tools: ToolbarActions {
        opened: true
        locked: true
        Action {
            text: "home"
        }
    }

    Component.onCompleted: {
        console.log("setting model " + model)
        //filterModel.model = model
    }

    Component.onDestruction: {
        console.log("model is " + model)
        model.exit();
    }

    // The delegate for each section header
    Component {
        id: sectionHeading
        Row {
            width: listView.width
            height: 20
            spacing: root.spacing

            Item {
                width: listView.width - sectionHeadingLabel.width - parent.spacing
                height: parent.height
                Rectangle {
                    width: parent.width - root.spacing * 2
                    height: 1
                    color: "#848684"
                    anchors.centerIn: parent
                }
            }

            Label {
                id: sectionHeadingLabel
                text: section
                font.bold: true
                fontSize: "small"
                anchors.verticalCenter: parent.verticalCenter
                color: "#848684"
            }
        }
    }

    Item {
        id: searchBar
        anchors.top: parent.top
        width: parent.width
        height: expanded ? searchTextField.height + root.spacing * 2 : 0
        property bool expanded

        Timer {
            id: searchTimer
            interval: 3000
            repeat: false
            running: searchBar.expanded && !searchTextField.activeFocus && filterModel.filter.length === 0

            onTriggered: {
                searchBar.expanded = false;
            }
        }


        Behavior on height {
            NumberAnimation { duration: 1500; easing.type: Easing.OutElastic }
        }
    }

    TextField {
        id: searchTextField
        anchors { left: parent.left; right: parent.right; bottom: listView.top }
        anchors.leftMargin: root.spacing
        anchors.rightMargin: root.spacing
        anchors.bottomMargin: listView.contentY + root.spacing
        opacity: searchBar.expanded ? 1 : 0
        enabled: searchBar.expanded
        z: 2
        primaryItem: Image {
            height: searchTextField.height - units.gu(1)
            width: height
            source: "/usr/share/icons/ubuntu-mobile/actions/scalable/filter.svg"
        }

        Keys.onReturnPressed: {
            listView.forceActiveFocus();
            platformCloseSoftwareInputPanel();
        }

        Behavior on opacity {
            NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
        }

        onTextChanged: {
            filterModel.filter = text;
        }
    }

    XbmcFilterModel {
        id: filterModel
        model: root.model
        caseSensitive: false

        // When the model is filtered, contentY is messed up sometimes
        // Lets unset and reset the model to keep the searchbar in place
        onFilterChanged: {
            listView.model = undefined
            listView.model = filterModel
        }
    }

    ListView {
        id: listView
        anchors {left: parent.left; top: searchBar.bottom; right: parent.right; bottom: parent.bottom; bottomMargin: units.gu(8)}
        clip: true
        //snapMode: ListView.SnapToItem
        //        header: listHeader
        highlightFollowsCurrentItem: true
        cacheBuffer: itemHeight * 3
        model: filterModel

        property bool draggedForSearch: root.model.allowSearch && contentY < -units.gu(15)
        property bool useThumbnails: settings.useThumbnails

        property int itemHeight: root.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? units.gu(10) : units.gu(8)

        signal collapse()

        onDraggedForSearchChanged: {
            searchBar.expanded = true;
        }

        delegate:  ListItems.Subtitled {
            id: delegateItem
            height: listView.itemHeight
            width: listView.width

            text: title
            subText: subtitle
            progression: filetype == "directory"

            icon: Item {
                width: thumbnailImage.width
                UbuntuShape {
                    id: thumbnailImage
                    height: listView.itemHeight - units.gu(2)
                    width: root.model.thumbnailFormat === XbmcModel.ThumbnailFormatPortrait ? height * 3/4 : (root.model.thumbnailFormat === XbmcModel.ThumbnailFormatLandscape ? height * 16/9 : height)
                    anchors.centerIn: parent
                    image: Image {
                        anchors.fill: parent
                        source: thumbnail
                    }
                }
            }

            onClicked: {
                if(filetype === "directory") {
                    var component = Qt.createComponent("BrowserPage.qml")
                    if (component.status === Component.Ready) {
                        var newModel = root.model.enterItem(filterModel.mapToSourceIndex(index));
                        newModel.ignoreArticle = settings.ignoreArticle;
                        pageStack.push(component, {model: newModel});
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                } else {
                    root.model.playItem(filterModel.mapToSourceIndex(index));
                }
            }

            onPressAndHold: {
                root.model.fetchItemDetails(filterModel.mapToSourceIndex(index))
                openEffect.positionPx = delegateItem.y - listView.contentY
                print("***._", delegateItem.y, listView.contentY, listView.itemHeight, openEffect.positionPx)
                openEffect.gap = 1.0
                itemDetailsLoader.selectedItem = root.model.getItem(filterModel.mapToSourceIndex(index))
            }
        }



//            Item {
//                id: expandedContent
//                anchors {left: parent.left; top: itemRow.bottom; right: parent.right}
//                anchors.bottom: parent.bottom
//                anchors.leftMargin: 15
//                width: parent.width - 20

//                Loader {
//                    id: contentLoader
//                    anchors.fill: parent
//                    onLoaded: print("loaderheight: " + childrenRect.height)

//                    Connections {
//                        target: contentLoader.item

//                        onPlayItem: {
//                            print("playItem()!")
//                            root.model.playItem(filterModel.mapToSourceIndex(index))
//                        }

//                        onAddToPlaylist: {
//                            root.model.addToPlaylist(filterModel.mapToSourceIndex(index))
//                        }

//                        onDownload: {
//                            root.model.download(filterModel.mapToSourceIndex(index), "/home/user/MyDocs/");
//                        }
//                    }
//                }
//                Behavior on opacity {
//                    NumberAnimation { duration: 300 }
//                }
//            }

//            Image {
//                id: arrow
//                //source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
//                anchors.right: parent.right;
//                anchors.verticalCenter: parent.verticalCenter
//                visible: filetype === "directory" && listItem.state == "collapsed" ? true : false;
//                opacity: .5
//            }


//            states: [
//                State {
//                    name: "expanded"
//                    PropertyChanges { target: listItem; height: listView.height; clip: true }
//                    //                    PropertyChanges { target: listView; snapMode: ListView.SnapOneItem }
//                    PropertyChanges { target: background; visible: false }
//                    PropertyChanges { target: expandedContent; opacity: 1 }
//                    PropertyChanges { target: contentLoader; source: "ItemDetails.qml" }
//                    PropertyChanges { target: listView; interactive: false; contentY: listView.itemHeight * listView.currentIndex }
//                    PropertyChanges { target: fastScroller; enabled: false }
////                    PropertyChanges { target: mouseArea; enabled: false }
//                }

//            ]

//            Behavior on height {
//                NumberAnimation {
//                    easing.type: Easing.InOutQuad
//                    duration: 300
//                }
//            }
//        }
        section.property: "sortingTitle"
        section.criteria: ViewSection.FirstCharacter
        // section.delegate: model.parentModel() != null ? sectionHeading : undefined

        Behavior on contentY {
            NumberAnimation {
                easing.type: Easing.OutQuad
                duration: 300
            }
        }

    }

    MouseArea {
        id:fastScroller
        anchors {top: searchBar.bottom; right: parent.right; bottom: parent.bottom }
        width: 75

        Rectangle {
            id: scrollBackground
            color: "black"
            opacity: 0
            anchors.fill: parent

            Behavior on opacity {
                NumberAnimation {
                    easing.type: Easing.InQuad
                    duration: 200
                }
            }
        }

        onReleased: {
            scrollBackground.opacity = 0;
        }

        onCanceled: {
            scrollBackground.opacity = 0;
        }

        onMouseYChanged: {
            scrollBackground.opacity = 0.2;
            var percent = Math.min(model.count - 1, Math.max(0, Math.round((mouseY) / fastScroller.height * (listView.count - 1))))
            scrollIndicatorLabel.text = model.get(percent, "sortingTitle").substring(0, 1);
            listView.positionViewAtIndex(percent, ListView.Center);
            scrollIndicator.y = Math.min(listView.height - scrollIndicator.height + listView.y, Math.max(listView.y, mouseY + fastScroller.y - scrollIndicator.height / 2))
        }

    }
    Item {
        id: scrollIndicator
        opacity: scrollBackground.opacity * 4
        anchors.left: parent.left
        anchors.right: parent.right
        height: 150

        Rectangle {
            anchors.fill: parent
            color: "black"
        }
        Label {
            id: scrollIndicatorLabel
            anchors.fill: scrollIndicator
            verticalAlignment: Text.AlignVCenter
            anchors.margins: 10
            color: "white"
            font.pixelSize: 64
        }
    }

    ActivityIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: model.busy
        visible: model.busy
    }

    OpenEffect {
        id: openEffect
        anchors.fill: parent
        anchors.bottomMargin: -bottomOverflow
        sourceItem: listView

        topGapPx: targetTopGapPx + (1 - gap) * (positionPx - targetTopGapPx)
        topOpacity: Math.max(0, (1 - gap * 0.8))
        bottomGapPx: positionPx + gap * (targetBottomGapPx - positionPx)
        bottomOverflow: units.gu(6)
        bottomOpacity: 1 - (gap * 0.8)

        property int targetTopGapPx: units.gu(10) // The page header might be there...
        property int targetBottomGapPx: root.height - listView.itemHeight
        property real gap: 0.0

        Behavior on gap {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: itemDetailsLoader.status == Loader.Ready
        onClicked: openEffect.gap = 0
    }

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: openEffect.bottomGapPx
        opacity: openEffect.gap * 0.3
        color: "black"

    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: openEffect.topGapPx
        opacity: openEffect.gap * 0.3
        color: "black"

    }

    Loader {
        id: itemDetailsLoader
        height: openEffect.bottomGapPx - openEffect.topGapPx
        anchors {
            top: parent.top
            topMargin: openEffect.topGapPx
            left: parent.left
            right: parent.right
        }
        source: openEffect.gap > 0 ? "ItemDetails.qml" : ""

        property variant selectedItem

        onItemChanged: {
            print("setting item to", selectedItem)
            item.selectedItem = selectedItem
        }
        onSelectedItemChanged: {
            print("selitemchanged", selectedItem)
            item.selectedItem = selectedItem
        }
    }
}
