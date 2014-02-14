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
        anchors.bottom: label.top
        anchors.topMargin: Theme.paddingLarge
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter

        opacity: 0.4

        source: cover.hasThumbnail > 0 ? cover.currentItem.thumbnail : "../icons/xbmcremote.png"
        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: label
        anchors.centerIn: parent
        width: parent.width - 2*Theme.paddingLarge
        color: Theme.secondaryColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        fontSizeMode: Text.Fit

        text: xbmc.connected ? xbmc.connectedHostName : qsTr("XBMC remote") + "\n" +
                               qsTr("Disconnected")
    }

    Loader {
        id: actionsLoader
    }

    states: [
        State {
            when: cover.player && (cover.player.state === "playing" || cover.player.state === "paused")
            PropertyChanges { target: actionsLoader; sourceComponent: playingActionsComponent }
        },
        State {
            when: !xbmc.connected
            PropertyChanges { target: actionsLoader; sourceComponent: disconnectedActionsComponent }
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


