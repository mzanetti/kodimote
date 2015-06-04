/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
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

import QtQuick 2.4
import Ubuntu.Components 1.2
import Ubuntu.Components.ListItems 1.0

Item {
    id: itemDetails

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
            fill: parent
            leftMargin: units.gu(1)
            rightMargin: units.gu(1)
        }
        contentWidth: width
        contentHeight: labelColumn.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        z: 5

        Column {
            id: labelColumn
            spacing: units.gu(1)
            height: childrenRect.height + units.gu(2)
            anchors {
                top: parent.top
                topMargin: units.gu(1)
                left: parent.left
                right: parent.right
            }

            LazyImage {
                width: parent.width
                height: Math.min(implicitHeight, itemDetails.width)
                scaleTo: "fit"
                source: "file://" + largeThumbnail
                initialWidth: parent.width
                initialHeight: width
            }

            Row {
                width: parent.width; spacing: 10; visible: rating > -1
                Label { id: ratingLabel; text: qsTr("Rating:"); font.bold: true }
                property int starCount: rating > 10 ? Math.round(rating / 20) : Math.round(rating / 2)
                RatingStars {
                    count: 5
                    rated: parent.starCount
                    height: ratingLabel.height
                    visible: currentItem !== null && (currentItem.type === "movie" || currentItem.type === "unknown")
                }
            }

            Row {
                width: parent.width; spacing: 10; visible: title.length > -1
                Label { text: qsTr("Title:"); font.bold: true }
                Label { text: title }
            }

            Row {
                width: parent.width; spacing: 10; visible: season > -1
                Label { text: qsTr("Season:"); font.bold: true }
                Label { text: season }
            }

            Row {
                width: parent.width; spacing: 10; visible: episode > -1
                Label { text: qsTr("Episode:"); font.bold: true }
                Label { text: episode }
            }

            Row {
                width: parent.width; spacing: 10; visible: firstaired > 0
                Label { text: qsTr("First aired:"); font.bold: true }
                Label { text: firstaired }
            }

            Row {
                width: parent.width; spacing: 10; visible: genre.length > 0
                Label { id: genreLabel; text: qsTr("Genre:"); font.bold: true }
                Label { text: genre; width: parent.width - genreLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: year > 0
                Label { text: qsTr("Year:"); font.bold: true }
                Label { text: year }
            }

            Row {
                width: parent.width; spacing: 10; visible: director.length > 0
                Label { id: directorLabel; text: qsTr("Director:"); font.bold: true }
                Label { text: director; width: parent.width - directorLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: tagline.length > 0
                Label { id: taglineLabel; text: qsTr("Tagline:"); font.bold: true }
                Label { text: tagline; width: parent.width - taglineLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: mpaa.length > 0
                Label { text: qsTr("MPAA:"); font.bold: true }
                Label { text: mpaa }
            }

            Row {
                width: parent.width; spacing: 10; visible: instrument.length > 0
                Label { text: qsTr("Instrument:"); font.bold: true }
                Label { text: instrument }
            }

            Row {
                width: parent.width; spacing: 10; visible: style.length > 0
                Label { id: styleLabel; text: qsTr("Style:"); font.bold: true }
                Label { text: style; width: parent.width - styleLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: mood.length > 0
                Label { id: moodLabel; text: qsTr("Mood:"); font.bold: true }
                Label { text: mood; width: parent.width - moodLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: born.length > 0
                Label { text: qsTr("Born:"); font.bold: true }
                Label { text: born }
            }

            Row {
                width: parent.width; spacing: 10; visible: formed.length > 0
                Label { text: qsTr("Formed:"); font.bold: true }
                Label { text: formed }
            }

            Row {
                width: parent.width; spacing: 10; visible: died.length > 0
                Label { text: qsTr("Died:"); font.bold: true }
                Label { text: died }
            }

            Row {
                width: parent.width; spacing: 10; visible: disbanded.length > 0
                Label { text: qsTr("Disbanded:"); font.bold: true }
                Label { text: disbanded }
            }

            Row {
                width: parent.width; spacing: 10; visible: duration.length > 0
                Label { text: qsTr("Duration:"); font.bold: true }
                Label { text: duration }
            }

            Row {
                width: parent.width; spacing: 10; visible: comment.length > 0
                Label { id: commentLabel; text: qsTr("Comment:"); font.bold: true }
                Label { text: comment; width: parent.width - commentLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            Row {
                width: parent.width; spacing: 10; visible: playcount > 0
                Label { text: qsTr("Playcount:"); font.bold: true }
                Label { text: playcount }
            }

            Row {
                width: parent.width; spacing: 10; visible: cast.length > 0
                Label { id: castLabel; text: qsTr("Cast:"); font.bold: true }
                Label { text: cast; width: parent.width - castLabel.width - parent.spacing; wrapMode: Text.WordWrap }
            }

            ThinDivider { width: parent.width }

            Label {
                id: plotLabel; width: parent.width; visible: plot.length > 0
                text: plot;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            Label {
                id: descriptionLabel; width: parent.width; visible: description.length > 0
                text: description;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }
}
