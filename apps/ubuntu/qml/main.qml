import QtQuick 2.0
import Ubuntu.Components 0.1

Item {
    id: appWindow

    VisualItemModel {
        id: tabModel

        PageStack {
            id: pageStack
            width: appWindow.width
            height: appWindow.height
            property int pageMargin: 16
            property bool connected: xbmc.connected
            property bool nfcSheetOpen: false

            Component.onCompleted: pageStack.push(mainPage)

            MainPage {
                id: mainPage
            }
        }


        NowPlayingPage {
            id: nowPlayingPage
            height: appWindow.height
            width: appWindow.width
        }

        Rectangle {
            height: appWindow.height
            width: appWindow.width
            color: "blue"
        }

    }

    ListView {
        orientation: ListView.Horizontal
        anchors.fill: parent
        contentHeight: appWindow.height
        contentWidth: appWindow.width
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange

        model: tabModel
    }

}
