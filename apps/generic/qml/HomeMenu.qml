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

import Qt 4.7


BorderImage {
    id: homeMenu
    border.top: 15
    border.right: 15
    border.left: 15
    border.bottom: 15
    source: "images/HomeBlade.png"

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.left
    width: 290
    state: "closed"
    opacity: anchors.rightMargin == 0 ? 0 : 1

    property alias subMenuState: subMenu.state

    onActiveFocusChanged: {
        if(focus) {
            homeMenuList.forceActiveFocus();
        }
    }

    ListModel {
        id: homeMenuModel
        ListElement { stateName: "audio"}
        ListElement { stateName: "video"}
        ListElement { stateName: "pictures"}
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
        id: homeMenuList
        anchors.fill: parent
        model: homeMenuModel
        anchors.margins: 25
        anchors.verticalCenter: parent.verticalCenter
        preferredHighlightBegin: height / 2 - 55
        preferredHighlightEnd: height / 2
        highlightRangeMode: ListView.StrictlyEnforceRange
        currentIndex: xbmcBrowser.mediaState == "audio" ? 0 : xbmcBrowser.mediaState == "video" ? 1 : 2
        focus: true

        Keys.onDownPressed: {
            incrementCurrentIndex();
//            selected(currentIndex);
        }
        Keys.onUpPressed: {
            decrementCurrentIndex();
//            selected(currentIndex);
        }
        Keys.onRightPressed: {
            selected(currentIndex);
            subMenuList.forceActiveFocus();
        }

        function selected(index) {
            homeMenuList.currentIndex = index
            switch(index) {
            case 0:
                xbmcBrowser.mediaState = "audio"
                break;
            case 1:
                xbmcBrowser.mediaState = "video"
                break;
            case 2:
                xbmcBrowser.mediaState = "pictures"
                xbmcBrowser.viewState = "files"
                homeMenu.state = "closed"
                return;
            }
            homeMenu.state = "halfopen"
            subMenu.state = "open"
        }

        delegate: Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 70

            Text {
                id: textLabel
                text: homeMenuModel.title(index)
                anchors.fill: parent
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                Behavior on font.pixelSize {
                    NumberAnimation { duration: 200 }
                }
                Behavior on color { ColorAnimation { duration: 200 } }
                states: [
                    State {
                        name: "unselected"; when: index !== homeMenuList.currentIndex
                        PropertyChanges { target: textLabel; font.pixelSize: 40; color: "grey" }
                    },
                    State {
                        name: "selected"; when: index === homeMenuList.currentIndex
                        PropertyChanges { target: textLabel; font.pixelSize: 48; color: "white" }
                    }
                ]
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    homeMenuList.selected(index);
                }
            }
        }
    }

    Behavior on anchors.rightMargin {
        NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
    }

    states:  [
        State { name: "open"
            PropertyChanges { target: homeMenu; anchors.rightMargin: -homeMenu.width }
            PropertyChanges { target: cancelArea; anchors.leftMargin: 0 }
        },
        State { name: "halfopen"
            PropertyChanges { target: homeMenu; anchors.rightMargin: -homeMenu.width + 100; }
            PropertyChanges { target: cancelArea; anchors.leftMargin: 0 }
        }
    ]

    Item {
        id: subMenu
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.right
        anchors.leftMargin: -10
        width: 240
        state: "closed"
        clip: true

        BorderImage {
            id: subMenuBackground
            border.top: 15
            border.right: 15
            border.left: 15
            border.bottom: 15
            source: "images/HomeBladeSub.png"
            width: parent.width

            anchors {top: parent.top; bottom: parent.bottom; right: parent.left }

            Behavior on anchors.rightMargin {
                NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
            }
        }

        ListModel {
            id: subMenuModel
            ListElement { stateName: "files"}
            ListElement { stateName: "library"}
            // workaround: its not possible to have qsTr() in ListElements for now...
            function title(index) {
                if (title["text"] === undefined) {
                    title.text = [
                        qsTr("Files"),
                        qsTr("Library")
                    ]
                }
                return title.text[index];
            }
        }
        ListView {
            id: subMenuList
            anchors.fill: subMenuBackground
            model: subMenuModel
            anchors.margins: 25
            anchors.verticalCenter: parent.verticalCenter
            preferredHighlightBegin: height / 2 - 55
            preferredHighlightEnd: height / 2
            highlightRangeMode: ListView.StrictlyEnforceRange
            currentIndex: xbmcBrowser.viewState == "library" ? 1 : 0

            Keys.onDownPressed: {
                incrementCurrentIndex();
            }
            Keys.onUpPressed: {
                decrementCurrentIndex();
            }
            Keys.onLeftPressed: {
                openSubMenu();
            }
            function openSubMenu() {
                subMenu.state = "closed"
                homeMenu.state = "open"
                homeMenuList.forceActiveFocus();
            }

            Keys.onRightPressed: {
                enter(currentIndex);
            }
            function enter(index) {
                switch(index) {
                case 0:
                    xbmcBrowser.viewState = "files"
                    break;
                case 1:
                    xbmcBrowser.viewState = "library"
                    break;
                }
                homeMenu.state = "closed"
                subMenu.state = "closed"
            }

            delegate: Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: -15
                height: 70
                visible: (stateName === "library" && xbmcBrowser.mediaState == "pictures") ? false : true
                Image {
                    id: buttonBg
                    anchors.fill: parent
                }
                Text {
                    id: subMenuLabel
                    color: "white"
                    text: subMenuModel.title(index)
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        subMenuList.enter(index);
                    }
                }
                states: [
                    State {
                        name: "unselected"; when: index !== currentIndex
                        PropertyChanges { target: buttonBg; source: "images/button-nofocus.png" }
                    },
                    State {
                        name: "selected"; when: index === subMenuList.currentIndex
                        PropertyChanges { target: buttonBg; source: "images/button-focus.png" }
                    }
                ]
            }
        }

        states: State {
            name: "open"
            PropertyChanges { target: subMenuBackground; anchors.rightMargin: -subMenu.width + 30; }
        }


        Image {
            id: hasSub
            source: "images/HasSub.png"

            anchors.left: subMenuBackground.right
            anchors.leftMargin: -10
            anchors.verticalCenter: subMenuBackground.verticalCenter
    //            width: 10
            state: "closed"


            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if(subMenu.state == "open") {
                        subMenu.state = "closed"
                        homeMenu.state = "open"
                        homeMenuList.forceActiveFocus();
                    } else {
                        subMenu.state = "open"
                        homeMenu.state = "halfopen"
                        subMenuList.forceActiveFocus();
                    }
                }
            }

        }

    }


}
