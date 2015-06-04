import QtQuick 2.4
import Ubuntu.Components 1.2
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0
import QtQuick.Layouts 1.1
import Kodi 1.0

Dialog {
    id: root
    title: qsTr("Select user")

    ColumnLayout {
        anchors {
            left: parent.left
            right: parent.right
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            height: units.gu(30)
            clip: true
            currentIndex: kodi.profiles().currentProfileIndex

            model: kodi.profiles();

            delegate: Empty {
                width: parent.width
                selected: index == listView.currentIndex
                property bool lockCodeRequired: lockMode === KodiModel.LockModeNumeric || lockMode === KodiModel.LockModeAlphaNumeric;
                property alias lockCode: lockCodeField.text
                clip: true
                height: listView.currentIndex == index && lockCodeRequired ? units.gu(12) : units.gu(6)
                Behavior on height {
                    UbuntuNumberAnimation {}
                }

                onClicked: {
                    listView.currentIndex = index
                }
                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                        margins: units.gu(2)
                    }
                    text: title
                    color: "black"
                }
                RowLayout {
                    anchors {
                        left: parent.left
                        leftMargin: units.gu(2)
                        right: parent.right
                        rightMargin: units.gu(2)
                        top: parent.top
                        topMargin: units.gu(6)
                    }

                    Label {
                        text: qsTr("Lock code")
                        color: "black"
                    }
                    TextField {
                        id: lockCodeField
                        placeholderText: qsTr("Lock code")
                        echoMode: TextInput.Password
                        inputMethodHints: (lockMode === KodiModel.LockModeNumeric ? Qt.ImhDigitsOnly : 0) | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
                        onAccepted: confirmButton.clicked()
                        Layout.fillWidth: true
                    }
                }
            }
        }

        Button {
            id: confirmButton
            Layout.fillWidth: true
            text: qsTr("OK")
            enabled: !listView.currentItem.lockCodeRequired || listView.currentItem.lockCode
            onClicked: {
                listView.model.switchProfile(listView.currentIndex, listView.currentItem.lockCode)
                PopupUtils.close(root)
            }
        }
    }
}
