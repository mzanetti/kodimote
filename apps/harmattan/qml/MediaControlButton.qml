import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: 80
    height: 80

    property string platformIconId
    property string iconSource
    property string source
    property bool enabled: true
    property bool selected: false
    property string text: ""
    property bool showBorder: false
    property alias font: label.font

    signal clicked()

    Rectangle {
        id: borderRect
        anchors.fill: parent
        border.color: "white"
        border.width: 5
        radius: width / 2
        color: "black"
        visible: root.showBorder
    }
    Label {
        id: label
        anchors.centerIn: parent
        text: root.text
    }

    Image {
        anchors.centerIn: parent

        function handleIconSource(iconId, isEnabled, isSelected) {
            if (root.source.length > 0) {
                return root.source;
            }

            if (root.iconSource.length > 0) {
                return "image://theme/" + root.iconSource;
            }

            if (!isEnabled)
                iconId = iconId + "-dimmed";

            if (theme.inverted)
                iconId = iconId + "-white";

            if (isSelected)
                iconId = iconId + "-selected";

            return "image://theme/icon-m-" + iconId;
        }

        source: handleIconSource(root.platformIconId, root.enabled, root.selected)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked();
    }
}
