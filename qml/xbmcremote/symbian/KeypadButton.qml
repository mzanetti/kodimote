// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Image {
    id: button
    property string icon
    signal pressed()
    source: "icons/" + button.icon + ".png"
    MouseArea {
        id: upMouseArea
        anchors.fill: parent
        onClicked: button.pressed();
    }
    states: [
        State {
            when: upMouseArea.pressed;
            PropertyChanges { target: button; source: "icons/" + button.icon + "_pressed.png" }
        }
    ]
}
