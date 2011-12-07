import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Page {
    id: mainPage
    Label {
        id: label
        anchors.centerIn: parent
        width: parent.width - 100
        text: xbmc.connectionError
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter

//        visible: false
    }
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: label.bottom
        anchors.topMargin: 10
        text: qsTr("Connect")
        onClicked: {
            var component = Qt.createComponent("ConnectionSheet.qml")
            if (component.status == Component.Ready) {
                component.createObject(mainPage).open()
            } else {
                console.log("Error loading component:", component.errorString());
            }
        }
    }
    XbmcDiscovery {
        id: discovery
    }
}
