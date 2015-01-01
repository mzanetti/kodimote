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
import "../"

CoverBackground {
    id: cover
    property QtObject player: kodi.activePlayer
    property QtObject currentItem: player ? player.currentItem : null
    property bool hasThumbnail: cover.currentItem && cover.currentItem.thumbnail.length

    function addHost() {
        pageStack.clear();
        pageStack.push(appWindow.initialPage);
        pageStack.currentPage.showConnect(PageStackAction.Immediate);
        pageStack.currentPage.addHost();
        appWindow.activate();
    }

    function browseMusic() {
        pageStack.clear();
        pageStack.push(appWindow.initialPage);
        pageStack.currentPage.browse("music");
        appWindow.activate();
    }

    function browseVideo() {
        pageStack.clear();
        pageStack.push(appWindow.initialPage);
        pageStack.currentPage.browse("video");
        appWindow.activate();
    }

    function connectToHost() {
        pageStack.clear();
        pageStack.push(appWindow.initialPage);
        pageStack.currentPage.showConnect();
        appWindow.activate();
    }

    function playPause() {
        cover.player.playPause();
    }

    function stop() {
        cover.player.stop();
    }

    Image {
        source: "./background.png"
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
    }

    Image {
        id: thumbnail
        width: cover.hasThumbnail > 0 ? parent.width - 2*Theme.paddingLarge : 80

        anchors.top: parent.top
        anchors.bottom: description.top
        anchors.topMargin: Theme.paddingLarge
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        visible: cover.hasThumbnail
        source: cover.hasThumbnail > 0 ? cover.currentItem.thumbnail : ""
        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: description
        anchors.verticalCenter: cover.verticalCenter
        anchors.left: cover.left
        anchors.right: cover.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        color: Theme.primaryColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        fontSizeMode: Text.HorizontalFit
        height: lineCount * font.pixelSize
    }

    CoverActionList {
        id: actions

        CoverAction {
            id: leftAction
        }

        CoverAction {
            id: rightAction
        }
    }

    states: [
        State {
            when: cover.player && (cover.player.state === "playing" || cover.player.state === "paused")
            PropertyChanges {
                target: description
                text: cover.currentItem ? (cover.currentItem.title + "\n" + cover.currentItem.subtitle) : ""
            }
            PropertyChanges {
                target: leftAction
                iconSource: "image://theme/icon-cover-" + (cover.player && cover.player.state === "playing" ? "pause" : "play")
                onTriggered: playPause()
            }
            PropertyChanges {
                target: rightAction
                iconSource: "../icons/icon-cover-stop.png"
                onTriggered: stop()
            }
        },
        State {
            when: kodi.connected
            PropertyChanges {
                target: description
                text: qsTr("XBMC on") + "\n" + kodi.connectedHostName
            }
            PropertyChanges {
                target: leftAction
                iconSource: "image://theme/icon-l-music"
                onTriggered: browseMusic()
            }
            PropertyChanges {
                target: rightAction
                iconSource: "image://theme/icon-l-video"
                onTriggered: browseVideo()
            }
        },
        State {
            when: !kodi.connected
            PropertyChanges {
                target: description
                text: qsTr("XBMC remote") + "\n" +
                      qsTr("Disconnected")
            }
            PropertyChanges {
                target: leftAction
                iconSource: "image://theme/icon-cover-new"
                onTriggered: addHost()
            }
            PropertyChanges {
                target: rightAction
                iconSource: "image://theme/icon-cover-search"
                onTriggered: connectToHost()
            }
        }
    ]
}


