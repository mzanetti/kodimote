import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    Label {
        id: label
        anchors.centerIn: parent
        text: qsTr("Could not connect to xbmc.\n Please check your network connection.")

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
}
