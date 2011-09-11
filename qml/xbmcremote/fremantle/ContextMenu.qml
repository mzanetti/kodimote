import Qt 4.7

Item {
    id: contextMenu
    opacity: 0
    anchors.centerIn: parent
    width: 400; height: topImage.height + middleImage.height + bottomImage.height
    state: "hidden"
    property alias model: contextMenuList.model

    signal clicked(int index)

    MouseArea {
        anchors.fill: contextMenu
        onClicked: {
            console.log("yummi - Mousepresses taste delicious!")
        }
    }

    Image {
        id: topImage
        source: "images/DialogContextTop.png"
        anchors { left: parent.left; top: parent.top; right: parent.right }
    }
    Image {
        id: middleImage
        source: "images/DialogContextMiddle.png"
        anchors { left: parent.left; top: topImage.bottom; right: parent.right }
        height: contextMenuList.model.count * 64
    }
    Image {
        id: bottomImage
        source: "images/DialogContextBottom.png"
        anchors { left: parent.left; bottom: parent.bottom; right: parent.right }
    }
    Image {
        id: closeButton
        source: closeButtonArea.pressed ? "images/DialogCloseButton-focus.png" : "images/DialogCloseButton.png"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 15
//            height: 20; width: 40

    }
    MouseArea {
        id: closeButtonArea
        anchors.fill: closeButton
        onClicked: contextMenu.state = "hidden"
    }

    ListView {
        id: contextMenuList
        anchors.fill: middleImage
        anchors.margins: 5
        interactive: false

        delegate: Item {
            width: parent.width
            height: 64


            Image {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                source: delegateArea.pressed ? "images/button-focus.png" : "images/button-nofocus.png"
            }
            Text {
                text: menuEntry
                color: "white"
                anchors.centerIn: parent
                font.pixelSize: 28
            }
            MouseArea {
                id: delegateArea
                anchors.fill: parent
                onClicked: {
                    contextMenu.clicked(entryId)
                    contextMenu.state = "hidden"
                }
            }

        }
    }

    states: [
        State {
            name: "visible"
            PropertyChanges { target: contextMenu; opacity: 1 }
        }

    ]
}

