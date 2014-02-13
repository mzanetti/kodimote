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

    Drawer {
        id: drawer

        anchors.fill: parent
        backgroundSize: height

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

            Grid {
                anchors.fill: parent
                anchors.margins: Theme.paddingLarge
                columns: nowPlayingPage.isPortrait ? 1 : 2
                spacing: Theme.paddingMedium

                Thumbnail {
                    id: imageItem
                    artworkSource: currentItem ? currentItem.thumbnail : ""
                    height: nowPlayingPage.isPortrait ? parent.width : parent.height
                    width: nowPlayingPage.isPortrait ? parent.width : height
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    defaultText: currentItem ? currentItem.title : ""
                }

                Column {
                    id: textItem
                    width: nowPlayingPage.isPortrait ? parent.width : parent.width - imageItem.width - parent.spacing
                    height: nowPlayingPage.isPortrait ? parent.height - imageItem.height - parent.spacing : parent.height
                    spacing: Theme.paddingSmall

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
                            color: Theme.highlightColor
                            elide: Text.ElideRight
                            text: currentItem ? currentItem.title : ""
                        }

                        Label {
                            id: playlistItemLabel
                            anchors.right: parent.right
                            color: Theme.highlightColor
                            elide: Text.ElideRight
                            text: playlist ? playlist.currentTrackNumber + "/" + playlist.count : "0/0"
                        }
                    }

                    Label {
                        color: Theme.highlightColor
                        elide: Text.ElideRight
                        text: currentItem ?  (xbmc.state == "audio" ? currentItem.artist : (currentItem.type == "episode" ? currentItem.tvShow : qsTr("Year:") + " " + currentItem.year)) : ""
                    }

                    Label {
                        color: Theme.highlightColor
                        elide: Text.ElideRight
                        text: currentItem ? (xbmc.state == "audio" ? currentItem.album : (currentItem.type == "episode" ? qsTr("Season:") + " " + currentItem.season + "   " + qsTr("Episode:") + " " + currentItem.episode : qsTr("Rating:") + " ")) : ""
                    }

                    PlayerControls {
                        anchors.horizontalCenter: parent.horizontalCenter
                        player: nowPlayingPage.player
                    }
                }
            }
        }
    }
}
