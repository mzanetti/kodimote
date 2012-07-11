import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

Sheet {
    id: root
    property alias titleText: title.text
    property string type

    property string inputedText

    acceptButtonText: qsTr("Send")
    rejectButtonText: qsTr("Cancel")

    SipAttributes {
        id: sipAttributes
    }

    content: Column {
        anchors.fill: parent
        spacing: 10

        Label {
            id: title
            font.pixelSize: 28
            width: parent.width
        }

        Item {
            id: keyboard
            width: parent.width
            visible: false
            height: childrenRect.height

            TextField {
                id: text
                anchors { left: parent.left; right: parent.right; }
                platformSipAttributes: sipAttributes

                Keys.onReturnPressed: root.accept()
            }
        }
    }

    states: [
        State {
            name: "keyboard"
            when: type == "keyboard"
            PropertyChanges {
               target: keyboard
               visible: true
            }
            PropertyChanges {
                target: text
                text: ""
            }
            PropertyChanges {
                target: root
                inputedText: {
                    return text.text;
                }
            }
            StateChangeScript {
                script: {
                    text.forceActiveFocus();
                }
            }
        },
        State {
            name: "number"
            when: type == "number"
            PropertyChanges {
                target: keyboard
                visible: true
            }
            PropertyChanges {
                target: text
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                text: ""
            }
            PropertyChanges {
                target: root
                inputedText: {
                    return text.text;
                }
            }
            StateChangeScript {
                script: {
                    text.forceActiveFocus();
                }
            }
        }
    ]

    onAccepted: {
        xbmc.keys().sendText(inputedText);
        type = "";
    }
}
