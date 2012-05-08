import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Page {
    id: mainPage
    tools: toolBarEntry

    XbmcDiscovery {
        id: discovery
    }

    Column {
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 100
            text: xbmc.connectionError
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
        Button{
            text: qsTr("Connect")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                var component = Qt.createComponent("ConnectionSheet.qml")
                if (component.status === Component.Ready) {
                    component.createObject(mainPage).open()
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
        }
    }
}
