import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: "OK"
    rejectButtonText: "Cancel"

    property string hostname

    content: Column {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        spacing: 10

        Label {
            text: "XBMC on " + hostname + "requires authentication:"
        }
        Label {
            text: "Username:"
        }
        TextField {
            width: parent.width
            id: username
//            text: xbmc.username

        }
        Label {
            text: "Password:"
        }
        TextField {
            width: parent.width
            id: password
//            text: xbmc.password
        }
    }

    onAccepted: {
        console.log("sheet accepted");
        xbmc.setAuthCredentials(username.text, password.text);
        authSheet.destroy();
    }

    onRejected: {
        print("rejected");
        authSheet.destroy();
    }
}
