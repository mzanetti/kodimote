import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: itemDetails
    width: parent.width
    height: parent.height
    //    height: childrenRect.height
    //    clip: true

    signal playItem
    signal addToPlaylist

    Component.onCompleted: print("**************itemHeight: " + height)

    Flickable {
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: buttonRow.top
        contentWidth: parent.width
        contentHeight: labelColumn.height
        clip: true
        z: 5

        Column {
            id: labelColumn
            spacing: 5
            height: childrenRect.height
            width: parent.width

            Row {
                width: parent.width; spacing: 10; visible: rating > -1
                Label { text: "Rating:"; font.bold: true }
                Label { text: rating }
            }

            Row {
                width: parent.width; spacing: 10; visible: season > -1
                Label { text: "Season:"; font.bold: true }
                Label { text: season }
            }

            Row {
                width: parent.width; spacing: 10; visible: episode > 1
                Label { text: "Episode:"; font.bold: true }
                Label { text: episode }
            }

            Row {
                width: parent.width; spacing: 10; visible: firstaired.length > 0
                Label { text: "First aired:"; font.bold: true }
                Label { text: firstaired }
            }

            Row {
                width: parent.width; spacing: 10; visible: genre.length > 0
                Label { text: "Genre:"; font.bold: true }
                Label { text: genre }
            }

            Row {
                width: parent.width; spacing: 10; visible: year.length > 0
                Label { text: "Year:"; font.bold: true }
                Label { text: year }
            }

            Row {
                width: parent.width; spacing: 10; visible: director.length > 0
                Label { text: "Director:"; font.bold: true }
                Label { text: director }
            }

            Row {
                width: parent.width; spacing: 10; visible: tagline.length > 0
                Label { text: "Tagline:"; font.bold: true }
                Label { text: tagline }
            }

            Row {
                width: parent.width; spacing: 10; visible: mpaa.length > 0
                Label { text: "MPAA:"; font.bold: true }
                Label { text: mpaa }
            }

            Row {
                width: parent.width; spacing: 10; visible: instrument.length > 0
                Label { text: "Instrument:"; font.bold: true }
                Label { text: instrument }
            }

            Row {
                width: parent.width; spacing: 10; visible: style.length > 0
                Label { text: "Style:"; font.bold: true }
                Label { text: model.style }
            }

            Row {
                width: parent.width; spacing: 10; visible: mood.length > 0
                Label { text: "Mood:"; font.bold: true }
                Label { text: mood }
            }

            Row {
                width: parent.width; spacing: 10; visible: born.length > 0
                Label { text: "Born:"; font.bold: true }
                Label { text: born }
            }

            Row {
                width: parent.width; spacing: 10; visible: formed.length > 0
                Label { text: "Formed:"; font.bold: true }
                Label { text: formed }
            }

            Row {
                width: parent.width; spacing: 10; visible: died.length > 0
                Label { text: "Died:"; font.bold: true }
                Label { text: died }
            }

            Row {
                width: parent.width; spacing: 10; visible: disbanded.length > 0
                Label { text: "Disbanded:"; font.bold: true }
                Label { text: disbanded }
            }

            Row {
                width: parent.width; spacing: 10; visible: duration.length > 0
                Label { text: "Duration:"; font.bold: true }
                Label { text: duration }
            }

            Row {
                width: parent.width; spacing: 10; visible: comment.length > 0
                Label { text: "Comment:"; font.bold: true }
                Label { text: comment }
            }

            Row {
                width: parent.width; spacing: 10; visible: playcount.length > 0
                Label { text: "Playcount:"; font.bold: true }
                Label { text: playcount }
            }


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

    ButtonRow {
        id: buttonRow
        anchors.bottom: parent.bottom
        width: parent.width
        exclusive: false
        Button {
            text: "Play"
            onClicked: itemDetails.playItem()
        }
        Button {
            text: "Add to playlist"
            onClicked: itemDetails.addToPlaylist()
        }
    }
}
