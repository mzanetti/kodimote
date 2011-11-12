import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.connectivity 1.2

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
        Label {
            text: "Username:"
        }
        TextField {
            width: parent.width
            id: username
            text: xbmc.username

        }
        Label {
            text: "Password:"
        }
        TextField {
            width: parent.width
            id: password
            text: xbmc.password
        }
    }
    Component.onCompleted: {
        console.log("port is" + xbmc.port)
    }

    onAccepted: {
        console.log("sheet accepted")
        xbmc.hostname = hostname.text;
        xbmc.port = port.text;
        xbmc.username = username.text;
        xbmc.password = password.text;
        xbmc.connectToHost();
    }
}
