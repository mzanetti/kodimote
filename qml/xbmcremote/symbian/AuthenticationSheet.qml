import QtQuick 1.1
import com.nokia.symbian 1.1
import Xbmc 1.0

Page {
    id: connectionSheet
//    acceptButtonText: qsTr("OK")
//    rejectButtonText: qsTr("Cancel")

    property string hostname

    Column {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        spacing: 10

        Label {
            width: parent.width
            text: qsTr("XBMC on %1 requires authentication:").arg(hostname);
            wrapMode: Text.WordWrap
        }
        Label {
            text: qsTr("Username:")
        }
        TextField {
            width: parent.width
            id: username
//            text: xbmc.username

        }
        Label {
            text: qsTr("Password:")
        }
        TextField {
            width: parent.width
            id: password
//            text: xbmc.password
        }
    }

//    onAccepted: {
//        console.log("sheet accepted");
//        xbmc.setAuthCredentials(username.text, password.text);
//        authSheet.destroy();
//    }

//    onRejected: {
//        print("rejected");
//        authSheet.destroy();
//    }

    tools: toolBarEntry
    ToolBarLayout {
        id: toolBarEntry
        visible: false
        ToolButton {
            text: "OK"
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                xbmc.setAuthCredentials(username.text, password.text);
                pageStack.pop();
            }
        }
        ToolButton {
            text: "Cancel"
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                pageStack.pop();
            }
        }
    }
}
