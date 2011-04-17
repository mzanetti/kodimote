import QtQuick 1.0

Item {
    id: browser
    anchors.fill: parent

    signal goBack

    BrowsingTopBar {
        id: topBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        currentDir: Files.currentDir

        onGoUp: Files.goUp(levels)
        onGoBack: browser.goBack()
    }

    BorderImage {
        border.top: 15
        border.right: 15
        border.left: 15
        border.bottom: 15

        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        source: "images/ContentPanel.png"

        ListView {
            id: list
            anchors.fill: parent
            anchors.margins: 25
            clip: true
            model: Files

            delegate: Item {
                width: parent.width
                height: 64
                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.top: parent.bottom
                    color: "white"
                    opacity: 0.5

                }

                Text {
                    color: "white"
                    text: label
                    font.pixelSize: 28
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        list.model.enterDir(directory)

                    }
                }
            }
        }

        ScrollBar {
            scrollArea: list; width: 15; alwaysVisible: false
            anchors { right: parent.right; top: parent.top; bottom: parent.bottom}
            anchors.rightMargin: 10
            anchors.topMargin: 20
            anchors.bottomMargin: 20
        }


    }
}
