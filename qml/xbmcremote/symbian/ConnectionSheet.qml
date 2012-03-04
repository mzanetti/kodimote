import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.connectivity 1.2
import Xbmc 1.0

Page {
    id: connectionSheet

//    XbmcDiscovery {
//        id: discovery
//        continuousDiscovery: true
//    }

    tools: toolBarEntry

    ToolBarLayout {
        id: toolBarEntry
        visible: false
        ToolButton {
            iconSource: "toolbar-back"
            onClicked: {
                if(itemView.currentIndex == 0) {
                    pageStack.pop();
                } else {
                    itemView.decrementCurrentIndex();
                }
            }
        }
        ToolButton {
            iconSource: "toolbar-add"
            visible: itemView.currentIndex == 0
            onClicked: {
                itemView.incrementCurrentIndex();
            }
        }
    }


        VisualItemModel {
            id: itemModel

            Column {
                width: itemView.width
                height: itemView.height

                ListView {
                    id: hostList
                    width: itemView.width
                    height: itemView.height
                    model: xbmc.hostModel()
                    highlightFollowsCurrentItem: true

                    Column {
                        anchors.centerIn: parent
                        width: parent.width - 10
                        height: searchLabel + busyIndicator + spacing
                        spacing: 20
                        visible: hostList.count == 0

                        BusyIndicator {
                            id: busyIndicator
                            anchors.horizontalCenter: parent.horizontalCenter
                            running: visible
                        }

                        Label {
                            id: searchLabel
                            width: parent.width
                            text: qsTr("Searching for XBMC hosts. Make sure that remote controlling capabilities in XBMC are enabled and announced using Zeroconf. If you don't use Zeroconf, add a host manually.")
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    highlight: Rectangle {
                        width: parent.width
                        height: 88
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: theme.inverted ? "#4269a5" : "#3996e7" }
                            GradientStop { position: 1.0; color: theme.inverted ? "#29599c" : "#1082de" }
                        }
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
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    xbmc.hostModel().wakeup(index);
                                    xbmc.hostModel().connectToHost(index);
                                }
                                onPressAndHold: {
                                    onClicked: {
                                        deleteQuestion.text = qsTr("Remove %1").arg(hostname);
                                        deleteQuestion.open();
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Column {
                height: itemView.height
                width: itemView.width
                Column {
                    height: parent.height - cancelManualAdd.height
                    width: parent.width

                    Label {
                        text: qsTr("Host:")
                    }
                    TextField {
                        width: parent.width
                        id: hostnameTextField

                    }
                    Item {
                        width: parent.width
                        height: 10
                    }

                    Label {
                        text: qsTr("HTTP Port:")
                    }
                    TextField {
                        width: parent.width
                        id: portTextField
                        inputMask: "00000"
                        inputMethodHints: Qt.ImhPreferNumbers
                    }
                    Item {
                        width: parent.width
                        height: 10
                    }
                    Label {
                        text: qsTr("MAC Address for WakeOnLan (optional):")
                    }
                    TextField {
                        width: parent.width
                        id: macTextField
                        inputMask: "HH:HH:HH:HH:HH:HH;_"
                        inputMethodHints: Qt.ImhPreferNumbers
                    }
                }
                Button {
                    id: cancelManualAdd
                    text: qsTr("Connect")
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        var newIndex = xbmc.hostModel().createHost(hostnameTextField.text, hostnameTextField.text, portTextField.text, macTextField.text);
                        xbmc.hostModel().connectToHost(newIndex);
                    }
                }
            }
        }
        ListView {
            id: itemView
            model: itemModel
            anchors.fill: parent

            orientation: ListView.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
            preferredHighlightBegin: 0; preferredHighlightEnd: 0
            highlightFollowsCurrentItem: true
            snapMode: ListView.SnapOneItem
            highlightMoveDuration: 200
            anchors.margins: appWindow.pageMargin
            interactive: false
        }


//    onAccepted: {
//        console.log("sheet accepted")
//        if(itemView.currentIndex == 0) {
//            xbmc.hostModel().wakeup(hostList.currentIndex);
//            xbmc.hostModel().connectToHost(hostList.currentIndex);
//        } else {
//            var newIndex = xbmc.hostModel().createHost(hostnameTextField.text, hostnameTextField.text, portTextField.text, macTextField.text);
//            xbmc.hostModel().connectToHost(newIndex);
//        }
//        connectionSheet.destroy();
//    }

//    onRejected: {
//        print("rejected");
//        connectionSheet.destroy();
//    }

        Dialog {
            id: deleteQuestion
            property alias text: label.text
            content: Label {
                id: label
            }
            buttons: ButtonRow {
                width: parent.width
                Button {
                    text: qsTr("Yes")
                    onClicked: {
                        xbmc.hostModel().removeHost(hostList.currentIndex);
                        deleteQuestion.close();
                    }
                }
                Button {
                    text: qsTr("No")
                    onClicked: deleteQuestion.close();
                }
            }
        }
}
