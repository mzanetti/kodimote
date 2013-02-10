import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import Xbmc 1.0

DefaultSheet {
    id: sheet
    title: "Select Host"
    doneButton: hostListView.currentIndex !== -1
    contentsWidth: units.gu(40)
    onDoneClicked: {
        xbmc.hostModel().connectToHost(hostListView.currentIndex)
        PopupUtils.close(sheet)
    }

    XbmcDiscovery {
    }

    container: ListView {
        id: hostListView
        anchors.fill: parent
        model: xbmc.hostModel()
        delegate: ListItems.Standard {
            text: hostname
            selected: index === ListView.view.currentIndex

            onClicked: ListView.view.currentIndex = index;
        }
    }
}

