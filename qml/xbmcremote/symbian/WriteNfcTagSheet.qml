import QtQuick 1.1
import com.nokia.symbian 1.1

Dialog {
    id: writeNfcTagSheet
    //acceptButtonText: "Close"
    property string errorMessage

    Component.onCompleted: {
        if(writeNfcTagSheet.errorMessage.length == 0) {
            nfcHandler.writeTag();
        }
    }

    Component.onDestruction: {
        print("nfc sheet destroyed");
        appWindow.nfcSheetOpen = false;
        nfcHandler.cancelWriteTag();
    }

    title: Column {
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: qsTr("Write NFC tag")
            width: parent.width
            font.pixelSize: 40
            color: "white"
        }
    }

    content: Item {
        width: parent.width
        height: 200
        Label {
            width: parent.width
            anchors.centerIn: parent
            id: textLabel
            text: writeNfcTagSheet.errorMessage.length == 0 ? qsTr("Tap a NFC tag to write XBMC connection information to it. You can then use the tag to connect to this XBMC.") : writeNfcTagSheet.errorMessage
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 25
        }
    }

    Connections {
        target: nfcHandler

        onTagWritten: {
            textLabel.text = statusText
            closeButton.text = qsTr("Close")
        }
    }

    onRejected: {
        console.log("sheet accepted")
        writeNfcTagSheet.destroy();
    }

    buttons {
        Button {
            id: closeButton
            width: parent.width
            text: writeNfcTagSheet.errorMessage.length == 0 ? qsTr("Cancel") : qsTr("Write NFC Tag");
            onClicked: {
                if(writeNfcTagSheet.errorMessage.length == 0) {
                    writeNfcTagSheet.reject();
                } else {
                    writeNfcTagSheet.errorMessage = "";
                    nfcHandler.writeTag();
                }
            }
        }
    }
}
