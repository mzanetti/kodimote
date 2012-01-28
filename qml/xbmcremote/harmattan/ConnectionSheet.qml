import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.connectivity 1.2
import Xbmc 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: qsTr("Connect")
    rejectButtonText: qsTr("Cancel")

    XbmcDiscovery {
        id: discovery
    }

    content: Column {
        anchors.fill: parent
        spacing: 10

        VisualItemModel {
            id: itemModel

            Column {
                width: itemView.width
                height: itemView.height

                ListView {
                    id: hostList
                    width: itemView.width
                    height: itemView.height - manualAdd.height
                    model: xbmc.hostModel()
                    highlightFollowsCurrentItem: true

                    Label {
                        anchors.centerIn: parent
                        text: qsTr("No XBMC hosts found.\nMake sure that remote controlling\ncapabilities are enabled and\nannounced using Zeroconf\nor connect to the host manually.")
                        visible: hostList.count == 0
                    }

                    highlight: Rectangle {
                        width: parent.width
                        height: 88
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: theme.inverted ? "#4269a5" : "#3996e7" }
                            GradientStop { position: 1.0; color: theme.inverted ? "#29599c" : "#1082de" }
                        }

                        //color: theme.inverted ? "#1d87e0"
                    }

                    delegate: Item {
                        width: parent.width
                        height: 88
                        Column {
                            anchors.fill: parent

                            Row {
                                id: itemRow
                                anchors {left: parent.left; top: parent.top; right: parent.right }
                                height: 88
                                anchors.leftMargin: 5
                                anchors.rightMargin: 5

                                Column {
                                    anchors.verticalCenter: parent.verticalCenter

                                    Label {
                                        id: mainText
                                        text: qsTr("XBMC on %1").arg(hostname)
                                        font.weight: Font.Bold
                                        font.pixelSize: 26
                                        width: itemRow.width
                                        elide: Text.ElideRight
                                    }

        //                            Label {
        //                                id: subText
        //                                text: ip + ":" + port
        //                                font.weight: Font.Light
        //                                font.pixelSize: 24
        //                                color: theme.inverted ? "#7b797b" : "#848684"
        //                                width: itemRow.width
        //                                elide: Text.ElideRight
        //                            }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    hostList.currentIndex = index;
                                }
                            }
                        }
                    }
                }
                Button {
                    id: manualAdd
                    text: qsTr("Manual connection")
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: itemView.incrementCurrentIndex()
                }
            }
            Column {
                height: itemView.height
                width: itemView.width

                Label {
                    text: qsTr("Host:")
                }
                TextField {
                    width: parent.width
                    id: hostnameTextField

                }
                Label {
                    text: qsTr("HTTP Port:")
                }
                TextField {
                    width: parent.width
                    id: portTextField
                }
            }


        }
        ListView {
            id: itemView
            model: itemModel
            anchors.fill: parent
            orientation: ListView.Horizontal
            snapMode: ListView.SnapOneItem
            anchors.margins: appWindow.pageMargin
            interactive: false
        }



    }

    onAccepted: {
        console.log("sheet accepted")
        if(itemView.currentIndex == 0) {
            xbmc.hostModel().wakeup(hostList.currentIndex);
            xbmc.hostModel().connectToHost(hostList.currentIndex);
        } else {
            var newIndex = xbmc.hostModel().createHost(hostnameTextField.text, hostnameTextField.text, portTextField.text);
            xbmc.hostModel().connectToHost(newIndex);
        }
        connectionSheet.destroy();
    }

    onRejected: {
        print("rejected");
        connectionSheet.destroy();
    }
}
