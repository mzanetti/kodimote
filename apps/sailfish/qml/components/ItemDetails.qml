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

Item {
    id: itemDetails
    anchors.fill: parent

    signal playItem
    signal addToPlaylist

    SilicaFlickable {
        id: flickable
        anchors {
            fill: parent
        }
        contentWidth: parent.width
        contentHeight: labelColumn.height + (labelColumn.y * 2)

        PullDownMenu {
            enabled: playable
            visible: enabled

            MenuItem {
                text: qsTr("Add to playlist")
                onClicked: itemDetails.addToPlaylist()
            }
            MenuItem {
                text: qsTr("Play")
                onClicked: itemDetails.playItem()
            }
        }

        Column {
            id: labelColumn
            spacing: 5
            height: childrenRect.height
            anchors {
                left: parent.left
                leftMargin: Theme.paddingSmall + Theme.paddingLarge
                right: parent.right
                rightMargin: Theme.paddingLarge
            }
            y: Theme.paddingMedium

            Row {
                width: parent.width; spacing: 10; visible: rating > -1
                Label { text: qsTr("Rating:"); font.bold: true; color: Theme.highlightColor }
                property int starCount: rating > 10 ? Math.round(rating / 20) : Math.round(rating / 2)
                Repeater {
                    model: parent.starCount
                    Image {
                        source: "image://theme/icon-m-favorite-selected"
                        height: Theme.fontSizeMedium
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Image {
                        source: "image://theme/icon-m-favorite"
                        height: Theme.fontSizeMedium
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            ItemDetailRow {
                visible: season > -1
                title: qsTr("Season:")
                text: season
            }

            ItemDetailRow {
                visible: episode > -1
                title: qsTr("Episode:")
                text: episode
            }

            ItemDetailRow {
                title: qsTr("First aired:")
                text: firstaired
            }

            ItemDetailRow {
                title: qsTr("Genre:")
                text: genre
            }

            ItemDetailRow {
                title: qsTr("Year:")
                text: year
            }

            ItemDetailRow {
                title: qsTr("Director:")
                text: director
            }

            ItemDetailRow {
                title: qsTr("Tagline:")
                text: tagline
            }

            ItemDetailRow {
                title: qsTr("MPAA:")
                text: mpaa
            }

            ItemDetailRow {
                title: qsTr("Instrument:")
                text: instrument
            }

            ItemDetailRow {
                title: qsTr("Style:")
                text: model.style
            }

            ItemDetailRow {
                title: qsTr("Mood:")
                text: mood
            }

            ItemDetailRow {
                title: qsTr("Born:")
                text: born
            }

            ItemDetailRow {
                title: qsTr("Formed:")
                text: formed
            }

            ItemDetailRow {
                title: qsTr("Died:")
                text: died
            }

            ItemDetailRow {
                title: qsTr("Disbanded:")
                text: disbanded
            }

            ItemDetailRow {
                title: qsTr("Duration:")
                text: duration
            }

            ItemDetailRow {
                title: qsTr("Comment:")
                text: comment
            }

            ItemDetailRow {
                title: qsTr("Playcount:")
                text: playcount
            }

            ItemDetailRow {
                title: qsTr("Cast:")
                text: cast
            }

            Label {
                id: plotLabel; width: parent.width; visible: plot.length > 0
                text: plot;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: Theme.highlightColor
            }
            Label {
                id: descriptionLabel; width: parent.width; visible: description.length > 0
                text: description;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: Theme.highlightColor
            }
        }

        VerticalScrollDecorator { }
    }
}
