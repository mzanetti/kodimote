// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.symbian 1.1

Rectangle {
    id: listHeader
    anchors {left: parent.left; top: parent.top; right: parent.right }
    height: 50
    z: 1
    property alias text: label.text

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#3c3c3c" }
//        GradientStop { position: 0.33; color: "yellow" }
        GradientStop { position: 1.0; color: "#151515" }
    }

    Label {
        id: label
        anchors.margins: 10
        anchors.fill: parent
        anchors.verticalCenter: listHeader.verticalCenter
        font.pixelSize: 28
        elide: Text.ElideLeft
    }
}
