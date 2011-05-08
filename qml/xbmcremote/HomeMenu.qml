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
        ListElement { label: "Music Files"}
        ListElement { label: "Music Library"}
        ListElement { label: "Video Files"}
        ListElement { label: "Video Library"}
    }
    ListView {
        id: musicSelectorView
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
            height: 70

            Text {
                color: "white"
                text: label
                font.pixelSize: 40
                anchors.fill: parent
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    switch(index) {
                    case 0:
                        xbmcBrowser.state = "audiofiles"
                        break;
                    case 1:
                        xbmcBrowser.state = "audiolibrary"
                        break;
                    case 2:
                        xbmcBrowser.state = "videofiles"
                        break;
                    case 3:
                        xbmcBrowser.state = "videolibrary"
                        break;
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
            PropertyChanges { target: cancelArea; anchors.leftMargin: 0 }
        }
    ]
}
