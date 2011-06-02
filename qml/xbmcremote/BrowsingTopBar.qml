import QtQuick 1.0

Item {
    id: topBar
//    source: "images/TopPanel.png"
    height: 70
    width: parent.width
    anchors.leftMargin: 10
    anchors.rightMargin: 10
//    border.left: 15; border.top: 0
//    border.right: 15; border.bottom: 15


    property string currentDir

    signal goUp(int levels)
    signal goBack

    function calculateDepth() {
        var temp = new Array()
        temp = currentDir.split("/");
        console.log("depth is " + temp.length)
        return 5;//temp.length
    }

    Item {
        id: backArrow
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.left: parent.left
        width: 60
        z: dirRow.count
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
            source: "images/HomeIcon.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: topBar.goBack()
        }
    }

    Row {
        anchors.top: parent.top
        anchors.bottom:  parent.bottom
        anchors.left:  backArrow.right
        anchors.right: parent.right
        Repeater {
            id: dirRow
            model: currentDir.split("/").length - 1
            delegate: Item {
                width: calculateWidth(folderName.text)
                height: 60
                z: dirRow.count - index

                function calculateWidth(text) {
                    var textElement = Qt.createQmlObject('import QtQuick 1.0; Text { font.pixelSize: 28; text: "' + text + '"}', parent, "calcTextWidth")
                    console.log("textElement is" + textElement.width)
                    var width = Math.min(textElement.width + 25, headerImage.width - 25)
                    textElement.destroy()
                    return width
                }

                Image {
                    id: headerImage
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: 60
                    source: "images/header.png"
                }

                Text {
                    id: folderName
                    width: parent.width - 25
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentDir.split("/")[index]
                    elide: Text.ElideRight
                    font.pixelSize: 28
                    color: "#0084ff"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    z: dirRow.count
                }

                MouseArea {
                    anchors.fill: folderName
                    onClicked: {
                        console.log("index is " + index + " count is " + currentDir.split("/").length )
                        var levels = currentDir.split("/").length - index - 2;
                        topBar.goUp(levels)
                    }
                }

            }
        }
    }
}
