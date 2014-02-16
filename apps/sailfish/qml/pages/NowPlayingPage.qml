/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
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
import "../components/"

Page {
    id: nowPlayingPage

    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player ? player.playlist() : null
    property QtObject currentItem: player ? player.currentItem : null

    onPlayerChanged: {
        if(player === null) {
            if(nowPlayingPage.status === PageStatus.Active) {
                //pop immediately to prevent the empty page being shown during the pop animation
                pageStack.pop(undefined, PageStackAction.Immediate);
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("PlaylistPage.qml"));
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuPlayerControls {

            }

            MenuItem {
                text: qsTr("Media")
                onClicked: {
                    pageStack.pop();
                }
            }

            MenuItem {
                text: qsTr("Now playing")
                enabled: false
            }
        }

        contentHeight: column.childrenRect.height

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Theme.paddingLarge

            Thumbnail {
                artworkSource: currentItem ? currentItem.thumbnail : ""
                width: parent.width
                height: parent.width
                fillMode: Image.PreserveAspectFit
                smooth: true
                defaultText: currentItem ? currentItem.title : ""

                MouseArea {
                    anchors.fill: parent
                    onClicked: drawer.open = !drawer.open
                }
            }

            Item {
                height: titleLabel.height
                anchors.left: parent.left
                anchors.right: parent.right
                Label {
                    id: titleLabel
                    anchors {
                        left: parent.left
                        right: playlistItemLabel.left
                        rightMargin: Theme.paddingMedium
                    }
                    font {
                        bold: true
                        family: Theme.fontFamilyHeading
                    }
                    elide: Text.ElideRight
                    text: currentItem ? currentItem.title : ""

                    MouseArea {
                        anchors.fill: parent
                        onClicked: drawer.open = !drawer.open
                    }
                }

                Label {
                    id: playlistItemLabel
                    anchors.right: parent.right
                    elide: Text.ElideRight
                    text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"

                    Behavior on opacity {
                        NumberAnimation {
                            property: "opacity"
                            duration: 300
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.navigateForward()
                    }
                }
            }

            Label {
                text: currentItem.subtitle
                width: parent.width
                elide: Text.ElideRight
                color: Theme.highlightColor
                visible: text.length > 0
                font {
                    family: Theme.fontFamilyHeading
                    bold: true
                }
            }

            Label {
                text: currentItem.album
                width: parent.width
                elide: Text.ElideRight
                visible: text.length > 0
                font.bold: true
            }

            ItemDetailRow {
                visible: currentItem.season > -1
                title: qsTr("Season:")
                text: currentItem.season
            }

            ItemDetailRow {
                visible: currentItem.episode > -1
                title: qsTr("Episode:")
                text: currentItem.episode
            }

            Drawer {
                id: drawer
                backgroundSize: itemDetails.height
                property real backgroundHeight: itemDetails.height * drawer._progress

                background: NowPlayingDetails {
                    id: itemDetails

                    width: parent.width
                }

                height: playerColumn.height < backgroundHeight ? backgroundHeight : playerColumn.height
                width: parent.width

                Column {
                    id: playerColumn

                    PlayerControls {
                        anchors.horizontalCenter: parent.horizontalCenter
                        player: nowPlayingPage.player
                    }
                }

                states: [
                    State {
                        when: drawer.opened
                        PropertyChanges { target: playlistItemLabel; opacity: 0 }
                    }
                ]
            }
        }
    }
}
