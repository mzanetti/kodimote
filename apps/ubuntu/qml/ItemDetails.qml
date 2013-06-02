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
import Ubuntu.Components.ListItems 0.1

Item {
    id: itemDetails
    //    height: childrenRect.height
    //    clip: true

    property variant selectedItem

    onSelectedItemChanged: print("äääääääääääääääitem changed", selectedItem, selectedItem.title)
    signal playItem
    signal addToPlaylist
    signal download

    function askDownload() {
        var component = Qt.createComponent(Qt.resolvedUrl("DownloadDialog.qml"));
        var dialog = component.createObject(itemDetails);
        dialog.itemName = title
        dialog.accepted.connect(download);
        dialog.open();
    }

    Component.onCompleted: print("**************itemHeight: " + height)

    Flickable {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: buttonLoader.top
            margins: units.gu(1)
        }
        contentWidth: width
        contentHeight: labelColumn.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        z: 5

        Column {
            id: labelColumn
            spacing: 5
            height: childrenRect.height
            anchors.left: parent.left
            anchors.right: parent.right

            UbuntuShape {
                // i.w : i.h = u.w : u.h
                width: parent.width
                height: width * img.sourceSize.height / img.sourceSize.width
                image: Image {
                    id: img
                    asynchronous: true
                    source: selectedItem.thumbnail
                }
            }

            Row {
                width: parent.width; spacing: 10; visible: rating > -1
                Label { text: qsTr("Rating:"); font.bold: true }
                property int starCount: rating > 10 ? Math.round(rating / 20) : Math.round(rating / 2)
                Repeater {
                    model: parent.starCount
                    Image {
                        source: theme.inverted ? "image://theme/meegotouch-indicator-rating-inverted-background-star" : "image://theme/meegotouch-indicator-rating-star"
                    }
                }
                Repeater {
                    model: 5 - parent.starCount
                    Image {
                        source: theme.inverted ? "image://theme/meegotouch-indicator-rating-inverted-background-star" : "image://theme/meegotouch-indicator-rating-star"
                        opacity: .4
                    }
                }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.title.length > -1
                Label { text: qsTr("Title:"); font.bold: true }
                Label { text: selectedItem.title }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.season > -1
                Label { text: qsTr("Season:"); font.bold: true }
                Label { text: selectedItem.season }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.episode > -1
                Label { text: qsTr("Episode:"); font.bold: true }
                Label { text: selectedItem.episode }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.firstaired > 0
                Label { text: qsTr("First aired:"); font.bold: true }
                Label { text: selectedItem.firstaired }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.genre.length > 0
                Label { id: genreLabel; text: qsTr("Genre:"); font.bold: true }
                Label { text: selectedItem.genre; width: parent.width - genreLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.year > 0
                Label { text: qsTr("Year:"); font.bold: true }
                Label { text: selectedItem.year }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.director.length > 0
                Label { id: directorLabel; text: qsTr("Director:"); font.bold: true }
                Label { text: selectedItem.director; width: parent.width - directorLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.tagline.length > 0
                Label { id: taglineLabel; text: qsTr("Tagline:"); font.bold: true }
                Label { text: selectedItem.tagline; width: parent.width - taglineLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.mpaa.length > 0
                Label { text: qsTr("MPAA:"); font.bold: true }
                Label { text: selectedItem.mpaa }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.instrument.length > 0
                Label { text: qsTr("Instrument:"); font.bold: true }
                Label { text: selectedItem.instrument }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.style.length > 0
                Label { id: styleLabel; text: qsTr("Style:"); font.bold: true }
                Label { text: selectedItem.style; width: parent.width - styleLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.mood.length > 0
                Label { id: moodLabel; text: qsTr("Mood:"); font.bold: true }
                Label { text: selectedItem.mood; width: parent.width - moodLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.born.length > 0
                Label { text: qsTr("Born:"); font.bold: true }
                Label { text: selectedItem.born }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.formed.length > 0
                Label { text: qsTr("Formed:"); font.bold: true }
                Label { text: selectedItem.formed }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.died.length > 0
                Label { text: qsTr("Died:"); font.bold: true }
                Label { text: selectedItem.died }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.disbanded.length > 0
                Label { text: qsTr("Disbanded:"); font.bold: true }
                Label { text: selectedItem.disbanded }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.duration.length > 0
                Label { text: qsTr("Duration:"); font.bold: true }
                Label { text: selectedItem.duration }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.comment.length > 0
                Label { id: commentLabel; text: qsTr("Comment:"); font.bold: true }
                Label { text: selectedItem.comment; width: parent.width - commentLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.playcount > 0
                Label { text: qsTr("Playcount:"); font.bold: true }
                Label { text: selectedItem.playcount }
            }

            Row {
                width: parent.width; spacing: 10; visible: selectedItem.cast.length > 0
                Label { id: castLabel; text: qsTr("Cast:"); font.bold: true }
                Label { text: selectedItem.cast; width: parent.width - castLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            ThinDivider { width: parent.width }

            Label {
                id: plotLabel; width: parent.width; visible: selectedItem.plot.length > 0
                text: selectedItem.plot;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            Label {
                id: descriptionLabel; width: parent.width; visible: description.length > 0
                text: description;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

    }
    Loader {
        id: buttonLoader
        anchors.bottom: parent.bottom
        width: parent.width
        states: [
            State {
                name: "hidden"; when: !playable
                PropertyChanges { target: buttonLoader; visible: false; height: 0 }
            },
            State {
                name: "horizontal"; when: screen.currentOrientation === Screen.Landscape
                PropertyChanges { target: buttonLoader; sourceComponent: horizontalButtons }
            },
            State {
                name: "vertical"; when: screen.currentOrientation === Screen.Portrait
                PropertyChanges { target: buttonLoader; sourceComponent: verticalbuttons }
            }

        ]
        onStateChanged: print("loader state is now:", state, itemDetails.orientation)
    }

    Component {
        id: horizontalButtons
        Row {
            id: buttonRow
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: parent.width

            states: [
                State {
                    name: "hidden"; when: !playable
                    PropertyChanges { target: buttonRow; visible: false; height: 0 }
                }
            ]

            Button {
                text: qsTr("Play")
                onClicked: itemDetails.playItem()
            }
            Button {
                text: qsTr("Add to playlist")
                onClicked: itemDetails.addToPlaylist()
            }
            Button {
                text: qsTr("Download")
                onClicked: itemDetails.askDownload()
            }
        }
    }

    Component {
        id: verticalbuttons
        Column {
            id: buttonColumn
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: parent.width

            Button {
                text: qsTr("Play")
                onClicked: itemDetails.playItem()
            }
            Button {
                text: qsTr("Add to playlist")
                onClicked: itemDetails.addToPlaylist()
            }
            Button {
                text: qsTr("Download")
                onClicked: itemDetails.askDownload()
            }
        }
    }

}
