import Qt 4.7

Image {
    id: button
    property string icon
    signal pressed()
    source: "images/" + button.icon + ".png"
    MouseArea {
        id: upMouseArea
        anchors.fill: parent
        onClicked: button.pressed();
    }
    states: [
        State {
            when: upMouseArea.pressed;
            PropertyChanges { target: button; source: "images/" + button.icon + "_pressed.png" }
        }
    ]
}
