import QtQuick 1.1
import com.meego 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: "Save"
    rejectButtonText: "Cancel"

    content: Column {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        spacing: 10
        Label {
            text: "Host:"
        }
        TextField {
            width: parent.width
            id: hostname
            text: xbmc.hostname

        }
        Label {
            text: "HTTP Port:"
        }
        TextField {
            width: parent.width
            id: port
            text: xbmc.port
        }
    }
    Component.onCompleted: {
        console.log("port is" + xbmc.port)
    }

    onAccepted: {
        console.log("sheet accepted")
        xbmc.hostname = hostname.text;
        xbmc.port = port.text;
        xbmc.connectToHost();
    }
}
