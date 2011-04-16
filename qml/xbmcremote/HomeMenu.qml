import QtQuick 1.0


BorderImage {
    id: homeMenu
    border.top: 15
    border.right: 15
    border.left: 15
    border.bottom: 15
    source: "images/HomeBlade.png"

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.left
    width: 200
    state: "closed"

    ListModel {
        id: musicSelectorModel
        ListElement { label: "Files"}
        ListElement { label: "Library"}
    }
    ListView {
        anchors.fill: parent
        model: musicSelectorModel
        anchors.margins: 25
        anchors.verticalCenter: parent.verticalCenter
        preferredHighlightBegin: height / 2 - 55
        preferredHighlightEnd: height / 2
        highlightRangeMode: ListView.StrictlyEnforceRange
        currentIndex: 0

        delegate: Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 55

            Text {
                color: "white"
                text: label
                font.pixelSize: 28
                anchors.fill: parent
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(index == 0) {
                        musicBrowser.state = "files"
                    } else {
                        musicBrowser.state = "library"
                    }
                    homeMenu.state = "closed"
                }
            }
        }
    }

    Behavior on anchors.rightMargin {
        NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
    }

    states:  [
        State { name: "open"
            PropertyChanges { target: homeMenu; anchors.rightMargin: -homeMenu.width }
        }
    ]
}
