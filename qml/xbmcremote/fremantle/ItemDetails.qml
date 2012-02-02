import QtQuick 1.0

FocusScope {
    id: detailsScope
    anchors.fill: parent
    state: "closed"
    opacity: 0
    property variant item

    onItemChanged: {
        print(" ************* item is now " + item.title)
    }

    function open() {
        detailsScope.forceActiveFocus();
    }

    MouseArea {
        id: backMouseArea
        anchors.fill: parent
        enabled: false

        onClicked: mainFocusArea.forceActiveFocus();

    }

    Keys.onPressed: {
        switch(event.key) {
        case Qt.Key_Backspace:
        case Qt.Key_Escape:
            mainFocusArea.forceActiveFocus();
            break;
        case Qt.Key_Down:
            if(flickableArea.contentY < flickableArea.contentHeight - height) {
                flickableArea.contentY += 10;
            }
            break;
        case Qt.Key_Up:
            if(flickableArea.contentY > 0) {
                flickableArea.contentY -= 10;
            }
            break;
        }

    }

    BorderImage {
        id: itemDetails
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15
        source: "images/HomeBlade.png"
        rotation: 180

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.right
        width: 600

        Behavior on anchors.leftMargin {
            PropertyAnimation {
                duration: 300
            }
        }
    }

    Flickable {
        id: flickableArea
        width: itemDetails.width - 60
        height: itemDetails.height - 60
        anchors.centerIn: itemDetails
        contentWidth: width
        contentHeight: labelColumn.height
        clip: true
        z: 5

        Column {
            id: labelColumn
            spacing: 5
            height: childrenRect.height
            width: parent.width

            Row {
                width: parent.width; spacing: 10; visible: item.rating > -1
                Text { text: qsTr("Rating:"); font.bold: true; color: "white" }
                Text { text: item.rating; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.season > -1
                Text { text: qsTr("Season:"); font.bold: true; color: "white" }
                Text { text: item.season; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.episode > 1
                Text { text: qsTr("Episode:"); font.bold: true; color: "white" }
                Text { text: item.episode; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.firstAired.length > 0
                Text { text: qsTr("First aired:"); font.bold: true; color: "white" }
                Text { text: item.firstAired; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.genre.length > 0
                Text { text: qsTr("Genre:"); font.bold: true; color: "white" }
                Text { text: item.genre; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.year.length > 0
                Text { text: qsTr("Year:"); font.bold: true; color: "white" }
                Text { text: item.year; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.director.length > 0
                Text { text: qsTr("Director:"); font.bold: true; color: "white" }
                Text { text: item.director; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.tagline.length > 0
                Text { text: qsTr("Tagline:"); font.bold: true; color: "white" }
                Text { text: item.tagline; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.mpaa.length > 0
                Text { text: qsTr("MPAA:"); font.bold: true; color: "white" }
                Text { text: item.mpaa; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.instrument.length > 0
                Text { text: qsTr("Instrument:"); font.bold: true; color: "white" }
                Text { text: item.instrument; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.style.length > 0
                Text { text: qsTr("Style:"); font.bold: true; color: "white" }
                Text { text: item.style; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.mood.length > 0
                Text { text: qsTr("Mood:"); font.bold: true; color: "white" }
                Text { text: item.mood; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.born.length > 0
                Text { text: qsTr("Born:"); font.bold: true; color: "white" }
                Text { text: item.born; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.formed.length > 0
                Text { text: qsTr("Formed:"); font.bold: true; color: "white" }
                Text { text: item.formed; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.died.length > 0
                Text { text: qsTr("Died:"); font.bold: true; color: "white" }
                Text { text: item.died; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.disbanded.length > 0
                Text { text: qsTr("Disbanded:"); font.bold: true; color: "white" }
                Text { text: item.disbanded; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.duration.length > 0
                Text { text: qsTr("Duration:"); font.bold: true; color: "white" }
                Text { text: item.duration; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.comment.length > 0
                Text { text: qsTr("Comment:"); font.bold: true; color: "white" }
                Text { text: item.comment; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.playcount.length > 0
                Text { text: qsTr("Playcount:"); font.bold: true; color: "white" }
                Text { text: item.playcount; color: "white" }
            }

            Row {
                width: parent.width; spacing: 10; visible: item.cast.length > 0
                Text { text: qsTr("Cast:"); font.bold: true; color: "white" }
                Text { text: item.cast; color: "white" }
            }


            Text {
                id: plotText; width: parent.width; visible: item.plot.length > 0; color: "white"
                text: item.plot;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            Text {
                id: descriptionText; width: parent.width; visible: item.description.length > 0; color: "white"
                text: item.description;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

    }

    states: [
        State {
            name: "open"; when: detailsScope.activeFocus
            PropertyChanges {
                target: detailsScope
                opacity: 1
            }
            PropertyChanges {
                target: itemDetails
                anchors.leftMargin: -600
            }
            PropertyChanges {
                target: backMouseArea
                enabled: true
            }
        }
    ]
}
