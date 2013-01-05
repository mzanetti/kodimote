import QtQuick 1.1
import com.nokia.meego 1.0

Dialog {
    id: downloadDialog
    property string itemName

    title: Item {
        height: 50
        width: parent.width
        Text {
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 32
            text: qsTr("Start download?")
        }

    }

    content:Item {
        id: name
        height: 100
        width: parent.width
        Label {
            id: text
            font.pixelSize: 22
            anchors.centerIn: parent
            color: "white"
            text: qsTr("Start downloading %1?").arg(downloadDialog.itemName)
        }
    }

    buttons {
        ButtonRow {
            width: parent.width
            Button {
                id: closeButton
                text: qsTr("Yes")
                onClicked: downloadDialog.accept();
            }
            Button {
                id: donateButton
                text: qsTr("No")
                onClicked: downloadDialog.reject();
            }
        }
    }
}
