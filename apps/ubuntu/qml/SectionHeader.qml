import QtQuick 2.0
import Ubuntu.Components 0.1

Row  {
    id: header
    property string headerText
    width: parent.width
    height: headerLabel.height
    opacity: .5
    Item {
        height: parent.height
        width: 20
    }

    Rectangle {
        height: 1
        width: parent.width - headerLabel.width - 40
        anchors.verticalCenter: parent.verticalCenter
        color: headerLabel.color
    }
    Item {
        height: parent.height
        width: 20
    }
    Label {
        id: headerLabel
        text: headerText
    }
}
