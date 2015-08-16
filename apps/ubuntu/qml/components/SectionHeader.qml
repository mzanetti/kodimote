import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3

RowLayout  {
    id: header
    property string headerText
    property alias color: headerLabel.color
    width: parent.width
    height: headerLabel.height
    opacity: .5
    spacing: units.gu(1)

    Rectangle {
        height: units.dp(1)
        Layout.fillWidth: true
        color: headerLabel.color
    }
    Label {
        id: headerLabel
        text: headerText
    }
}
