import QtQuick 1.0

BorderImage {
    id: topBar
    source: "images/TopPanel.png"
    height: 70
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    border.left: 15; border.top: 0
    border.right: 15; border.bottom: 15

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
        width: height
        Image {
            anchors.centerIn: parent
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
            model: currentDir.split("/").length
            delegate: Item {
                width: folderName.width + 10
                height: 60
                Text {
                    id: folderName
                    anchors.centerIn: parent
                    text: currentDir.split("/")[index] + (currentDir.split("/").length > index+1 ? " >" : "")
                    font.pixelSize: 28
                    color: "#0084ff"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
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
