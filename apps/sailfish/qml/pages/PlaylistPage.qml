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

Page {
    id: playlistPage

    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()

    PageHeader {
        id: header
        title: playlist.title
    }

    SilicaListView {
        id: listView

        anchors {
            top: header.bottom
            bottom: parent.bottom
            //margins: Theme.paddingMedium
        }
        width: parent.width
        model: playlist

        delegate: ListItem {
            id: listItem

            width: parent.width
            contentHeight: Theme.itemSizeMedium

            onClicked: {
                player.playItem(index);
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Play")
                    onClicked: {
                        player.playItem(index)
                    }
                }
                MenuItem {
                    text: qsTr("Remove from playlist")
                    onClicked: {
                        playlist.removeItem(index)
                    }
                }
                MenuItem {
                    text: qsTr("Clear playlist")
                    onClicked: {
                        playlist.clear()
                    }
                }
            }

            Column {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: durationLabel.left
                    leftMargin: Theme.paddingLarge
                }

                Label {
                    id: mainText
                    text: title
                    font.weight: Font.Bold
                    font.pixelSize: 26
                    width: listView.width - durationLabel.width
                    elide: Text.ElideRight
                    color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    states: [
                        State {
                            name: "highlighted"
                            when: index === listView.model.currentTrackNumber - 1
                            PropertyChanges {
                                target: mainText
                                color: Theme.highlightColor
                            }
                        }
                    ]
                }

                Label {
                    id: subText
                    text: subtitle
                    font.weight: Font.Light
                    font.pixelSize: 24
                    color: Theme.secondaryColor
                    width: listView.width - durationLabel.width
                    elide: Text.ElideRight
                    visible: text != ""
                }
            }

            Label {
                id: durationLabel
                text: duration
                anchors {
                    right: parent.right
                    rightMargin: Theme.paddingLarge
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        VerticalScrollDecorator {

        }
    }
}
