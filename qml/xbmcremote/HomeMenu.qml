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
    property alias subMenuState: subMenu.state

    ListModel {
        id: homeMenuModel
        ListElement { label: "Music"; stateName: "audio"}
        ListElement { label: "Video"; stateName: "video"}
    }
    ListView {
        id: homeMenuList
        anchors.fill: parent
        model: homeMenuModel
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
                id: textLabel
                text: label
                anchors.fill: parent
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                Behavior on font.pixelSize {
                    NumberAnimation { duration: 200 }
                }
                Behavior on color { ColorAnimation { duration: 200 } }
                states: [
                    State {
                        name: "unselected"; when: xbmcBrowser.mediaState != stateName
                        PropertyChanges { target: textLabel; font.pixelSize: 40; color: "grey" }
                    },
                    State {
                        name: "selected"; when: xbmcBrowser.mediaState == stateName
                        PropertyChanges { target: textLabel; font.pixelSize: 48; color: "white" }
                    }
                ]
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    homeMenuList.currentIndex = index
                    switch(index) {
                    case 0:
                        xbmcBrowser.mediaState = "audio"
                        break;
                    case 1:
                        xbmcBrowser.mediaState = "video"
                        break;
                    }
//                    homeMenu.state = "closed"
                    subMenu.state = "open"
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

    Item {
        id: subMenu
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.right
        anchors.leftMargin: -10
        width: 200
        state: "closed"
        clip: true

        BorderImage {
            id: subMenuBackground
            border.top: 15
            border.right: 15
            border.left: 15
            border.bottom: 15
            source: "images/HomeBladeSub.png"
            width: 200

            anchors {top: parent.top; bottom: parent.bottom; right: parent.left }

            Behavior on anchors.rightMargin {
                NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
            }
        }

        ListModel {
            id: subMenuModel
            ListElement { label: "Files"; stateName: "files"}
            ListElement { label: "Library"; stateName: "library"}
        }
        ListView {
            id: subMenuList
            anchors.fill: subMenuBackground
            model: subMenuModel
            anchors.margins: 25
            anchors.verticalCenter: parent.verticalCenter
            preferredHighlightBegin: height / 2 - 55
            preferredHighlightEnd: height / 2
            highlightRangeMode: ListView.StrictlyEnforceRange
            currentIndex: 0

            delegate: Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: -15
                height: 70
                Image {
                    id: buttonBg
                    anchors.fill: parent
                }
                Text {
                    id: subMenuLabel
                    color: "white"
                    text: label
                    font.pixelSize: 32
                    font.weight: Font.Bold
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        switch(index) {
                        case 0:
                            xbmcBrowser.viewState = "files"
                            break;
                        case 1:
                            xbmcBrowser.viewState = "library"
                            break;
                        }
                        homeMenu.state = "closed"
                        subMenu.state = "closed"
                    }
                }
                states: [
                    State {
                        name: "unselected"; when: xbmcBrowser.viewState != stateName
                        PropertyChanges { target: buttonBg; source: "images/button-nofocus.png" }
                    },
                    State {
                        name: "selected"; when: xbmcBrowser.viewState == stateName
                        PropertyChanges { target: buttonBg; source: "images/button-focus.png" }
                    }
                ]
            }
        }

        states: State {
            name: "open"
            PropertyChanges { target: subMenuBackground; anchors.rightMargin: -subMenu.width + 30 }
        }


        Image {
            id: hasSub
            source: "images/HasSub.png"

            anchors.left: subMenuBackground.right
            anchors.leftMargin: -10
            anchors.verticalCenter: subMenuBackground.verticalCenter
    //            width: 10
            state: "closed"


            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if(subMenu.state == "open") {
                        subMenu.state = "closed"
                    } else {
                        subMenu.state = "open"
                    }
                }
            }

        }

    }


}
