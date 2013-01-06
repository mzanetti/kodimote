import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: 80
    height: 64

    property string platformIconId
    property bool enabled: true
    property bool selected: false

    signal clicked()

    Image {
        anchors.centerIn: parent

        function handleIconSource(iconId, isEnabled, isSelected) {
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
