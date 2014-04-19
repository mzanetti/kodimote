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
import "../"

CoverBackground {
    id: cover
    property QtObject player: xbmc.activePlayer
    property QtObject currentItem: player ? player.currentItem : null
    property bool hasThumbnail: cover.currentItem && cover.currentItem.thumbnail.length

    Image {
        id: thumbnail
        width: cover.hasThumbnail > 0 ? parent.width - 2*Theme.paddingLarge : 80

        anchors.top: parent.top
        anchors.bottom: description.top
        anchors.topMargin: Theme.paddingLarge
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        opacity: 0.4

        source: cover.hasThumbnail > 0 ? cover.currentItem.thumbnail : "../icons/xbmcremote.png"
        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: description
        anchors.verticalCenter: cover.verticalCenter
        anchors.left: cover.left
        anchors.right: cover.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        color: Theme.secondaryColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        fontSizeMode: Text.HorizontalFit
        height: lineCount * font.pixelSize
    }

    Loader {
        id: actionsLoader
    }

    states: [
        State {
            when: cover.player && (cover.player.state === "playing" || cover.player.state === "paused")
            PropertyChanges {
                target: actionsLoader
                sourceComponent: playingActionsComponent
            }
            PropertyChanges {
                target: description
                text: cover.currentItem ? (cover.currentItem.title + "\n" + cover.currentItem.subtitle) : ""
            }
        },
        State {
            when: xbmc.connected
            PropertyChanges {
                target: actionsLoader
                sourceComponent: connectedActionsComponent
            }
            PropertyChanges {
                target: description
                text: qsTr("XBMC on") + "\n" + xbmc.connectedHostName
            }
        },
        State {
            when: !xbmc.connected
            PropertyChanges {
                target: actionsLoader
                sourceComponent: disconnectedActionsComponent
            }
            PropertyChanges {
                target: description
                text: qsTr("XBMC remote") + "\n" +
                      qsTr("Disconnected")
            }
        }

    ]

    Component {
        id: disconnectedActionsComponent

        CoverActionList {
            id: playingActions

            CoverAction {
                iconSource: "image://theme/icon-cover-new"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push(appWindow.initialPage);
                    pageStack.currentPage.showConnect(PageStackAction.Immediate);
                    pageStack.currentPage.addHost();
                    appWindow.activate();
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-search"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push(appWindow.initialPage);
                    pageStack.currentPage.showConnect();
                    appWindow.activate();
                }
            }
        }
    }

    Component {
        id: connectedActionsComponent

        CoverActionList {
            id: playingActions

            CoverAction {
                iconSource: "image://theme/icon-l-music"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push(appWindow.initialPage);
                    pageStack.currentPage.browse("music");
                    appWindow.activate();
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-l-video"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push(appWindow.initialPage);
                    pageStack.currentPage.browse("video");
                    appWindow.activate();
                }
            }
        }
    }

    Component {
        id: playingActionsComponent

        CoverActionList {
            id: playingActions

            CoverAction {
                iconSource: "../icons/icon-cover-stop.png"
                onTriggered: cover.player.stop()
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-" + (cover.player && cover.player.state === "playing" ? "pause" : "play")
                onTriggered: cover.player.playPause()
            }
        }
    }
}


