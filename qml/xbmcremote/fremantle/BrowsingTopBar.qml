import Qt 4.7

Item {
    id: topBar
//    source: "images/TopPanel.png"
    height: 70
    width: 500 //parent.width
    anchors.leftMargin: 10
    anchors.rightMargin: 10
//    border.left: 15; border.top: 0
//    border.right: 15; border.bottom: 15


    property string currentDir
    property string currentIcon: "images/HomeIcon.png"

    signal goUp
    signal goBack

    Item {
        id: backArrow
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.leftMargin: -10
        anchors.left: parent.left
        width: 80
        z: 1
        clip: true
        Image {
            id: header
            anchors.right: parent.right
            anchors.top: parent.top
            height: 60
            source: "images/header.png"
        }
        Image {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 20
            height: 32
            width: 32
            source: currentIcon
        }

        MouseArea {
            anchors.fill: parent
            onClicked: topBar.goBack()
        }
    }

    Item {
        id: dirItem
        anchors.top: parent.top
        anchors.bottom:  parent.bottom
        anchors.left:  backArrow.right
//        anchors.right: parent.right
        width: calculateWidth(folderName.text)

        BorderImage {
            id: headerImage
            anchors.right: parent.right
            anchors.top: parent.top
            height: 60
            width: parent.width + 25
            border.right: 25
            source: "images/header.png"
        }

        Text {
            id: folderName
            width: parent.width - 25
            anchors.left: parent.left
            anchors.verticalCenter: headerImage.verticalCenter
            text: currentDir
            elide: Text.ElideLeft
            font.pixelSize: 28
            color: "#0084ff"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
//            z: dirRow.count
        }
        MouseArea {
            anchors.fill: folderName
            onClicked: {
//                        console.log("index is " + index + " count is " + currentDir.split("/").length )
//                        var levels = currentDir.split("/").length - index - 2;
//                        topBar.goUp(levels)
                topBar.goUp()
            }
        }
    }

//    Row {
//        anchors.top: parent.top
//        anchors.bottom:  parent.bottom
//        anchors.left:  backArrow.right
//        anchors.right: parent.right
//        Repeater {
//            id: dirRow
//            model: currentDir.split("/").length - 1
//            delegate: Item {
//                id: dirEntry
//                width: calculateWidth(folderName.text)
//                height: 60
//                z: dirRow.count - index

                function calculateWidth(text) {
                    var textElement = Qt.createQmlObject('import Qt 4.7; Text { visible: false; font.pixelSize: 28; text: "' + text + '"}', dirItem, "calcTextWidth")
//                    console.log("textElement is" + textElement.width)
                    var width = Math.min(textElement.width + 25, parent.width - 25 - backArrow.width)
                    textElement.destroy()
                    return width
                }

//                Image {
//                    id: headerImage
//                    anchors.right: parent.right
//                    anchors.top: parent.top
//                    height: 60
//                    source: "images/header.png"
//                }

//                Text {
//                    id: folderName
//                    width: parent.width - 25
//                    anchors.left: parent.left
//                    anchors.verticalCenter: parent.verticalCenter
//                    text: currentDir.split("/")[index]
//                    elide: Text.ElideRight
//                    font.pixelSize: 28
//                    color: "#0084ff"
//                    horizontalAlignment: Text.AlignLeft
//                    verticalAlignment: Text.AlignVCenter
//                    z: dirRow.count
//                }


//            }
//        }
//    }
}
