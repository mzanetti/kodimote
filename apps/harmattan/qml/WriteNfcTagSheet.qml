import QtQuick 1.1
import com.nokia.meego 1.0

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

    content: Label {
        height: 200
        width: 400
        anchors.centerIn: parent
        id: textLabel
        text: writeNfcTagSheet.errorMessage.length == 0 ? qsTr("Tap a NFC tag to write XBMC connection information to it. You can then use the tag to connect to this XBMC.") : writeNfcTagSheet.errorMessage
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        font.pointSize: 25
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
        writeNfcTagSheet.deleteLater();
    }

    buttons {
        Button {
            id: closeButton
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
