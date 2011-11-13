import Qt 4.7

Item {
    id: browser
    anchors.fill: parent
    property alias model: list.model

    signal goBack

    BrowsingTopBar {
        id: topBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        currentDir: list.model.title
        function getTitle() {
            console.log("model is: " + list.model)
            var parentModel = list.model;
            console.log("tmp is: " + list.model + list.model.title)
            console.log("reantModel:" + parentModel.title)
            var title = "";
            while(parentModel !== null) {
                console.log("pm:" + parentModel)
                title = parentModel.title + "/" + title
                parentModel = parentModel.parentModel()
            }
            console.log("got title:" + title)
            return title;
        }

        onGoUp: model.goUp(levels)
        onGoBack: browser.goBack()
    }

    BorderImage {
        id: listBackground
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

            delegate: Item {
                width: parent.width
                height: 64
                Image {
                    anchors.fill: parent
                    source: "images/MenuItemNF.png"
                }

                Image {
                    id: thumbnailImage
                    height: parent.height - 2
    //                width: height
                    fillMode: Image.PreserveAspectFit
                    smooth: false
                    source: thumbnail
                    sourceSize.height: parent.height - 2
                    //visible: settings.useThumbnails
                }

                Text {
                    color: "white"
                    text: title
                    font.pixelSize: 28
                    anchors.fill: parent
                    anchors.leftMargin: thumbnailImage.width + 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        var newModel = list.model.enterItem(index)
                        console.log("newModel: " + newModel)
                        list.model = newModel
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

    states: [
        State {
            name: "hidden"; when: !visible
            PropertyChanges { target: listBackground; opacity: 0}
            PropertyChanges { target: topBar; opacity: 0; anchors.left: parent.right}
        },
        State {
            name: "visible"; when: visible
            PropertyChanges { target: listBackground; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "hidden"
            to: "visible"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.InQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  300; easing.type: Easing.OutQuad}
        },
        Transition {
            from: "visible"
            to: "hidden"
            NumberAnimation { target: listBackground; property: "opacity"; duration: 300; easing.type: Easing.OutQuint }
            NumberAnimation { target: topBar; property: "anchors.leftMargin"; duration:  200; easing.type: Easing.InQuad}
            NumberAnimation { target: topBar; property: "opacity"; duration: 300; easing.type: Easing.OutQuad}
        }
    ]


}
