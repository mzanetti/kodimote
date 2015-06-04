import QtQuick 2.4
import Ubuntu.Components 1.2

Row  {
    id: header
    property string headerText
    property alias color: headerLabel.color
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
