import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.0
import QtQuick.Layouts 1.1
import Kodi 1.0

Item {
    id: root
    anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
    height: units.gu(2)

    default property alias data: optionsRow.data

    Rectangle {
        anchors {fill: parent; topMargin: -units.gu(200) }
        color: "#88000000"
        opacity: root.progress
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.progress = 0;
                mouse.accepted = true;
            }
            enabled: root.progress > 0
        }
    }

    property real progress: 0

    Connections {
        target: pageStack
        onDepthChanged: {
            root.progress = 0;
        }
    }

    MouseArea {
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        height: contentRect.height
        property bool ignoring: false

        property var gesturePoints: new Array()

        onPressed: {
            gesturePoints = new Array();
            ignoring = false;
            if (root.progress == 0 && mouseY < height - units.gu(2)) {
                print("rejecting mouse")
                mouse.accepted = false;
                ignoring = true;
            }
        }

        onMouseYChanged: {
            if (ignoring) {
                return;
            }
            root.progress = Math.min(1, (height - mouseY) / height)
            gesturePoints.push(mouseY)
        }

        onReleased: {
            var oneWayFlick = true;
            var upwards = gesturePoints[1] < gesturePoints[0];
            for (var i = 1; i < gesturePoints.length; i++) {
                if (upwards && gesturePoints[i] > gesturePoints[i-1]) {
                    oneWayFlick = false;
                    break;
                } else if(!upwards && gesturePoints[i] < gesturePoints[i-1]) {
                    oneWayFlick = false;
                    break;
                }
            }

            if (oneWayFlick && upwards) {
                root.progress = 1;
            } else if (oneWayFlick && !upwards) {
                root.progress = 0;
            } else if (root.progress > .5) {
                root.progress = 1;
            } else {
                root.progress = 0;
            }
        }


        Rectangle {
            id: contentRect
            anchors {left: parent.left; right: parent.right; top: parent.bottom }
            anchors.topMargin: -units.gu(2) - root.progress * (height - units.gu(2))
            height: contentColumn.height + units.gu(4)
            Behavior on anchors.topMargin {
                UbuntuNumberAnimation {}
            }

            color: theme.palette.normal.overlay

            Rectangle {
                anchors { left: contentRect.left; top: contentRect.top; right: contentRect.right }
                height: units.dp(1)
                color: UbuntuColors.warmGrey
            }

            Rectangle {
                anchors.top: parent.top
                anchors.topMargin: units.gu(.5)
                anchors.horizontalCenter: parent.horizontalCenter
                height: units.gu(1)
                width: units.gu(6)
                radius: width / 2
                color: "lightgray"
            }

            MouseArea {
                anchors.fill: parent
                anchors.topMargin: units.gu(2)
                onPressed: {
                    mouse.accepted = true;
                }
            }

            ColumnLayout {
                id: contentColumn
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: units.gu(2) }

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: kodi.connectedHostName
                        Layout.fillWidth: true
                        color: "black"
                    }
                    BottomEdgeButton {
                        source: "image://theme/system-shutdown"
                        Layout.preferredWidth: undefined // overriding default to distribute space
                        onClicked: {
                            var dialog = PopupUtils.open(Qt.resolvedUrl("PowerMenu.qml"), root)
                            dialog.selectUser.connect(function() {
                                PopupUtils.open(Qt.resolvedUrl("ProfileSelectionDialog.qml"), root)
                            })
                        }
                    }
                }

                SectionHeader {
                    color: "black"
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    BottomEdgeButton {
                        source: "image://theme/audio-speakers-muted-symbolic"
                        Layout.fillWidth: !volumeSlider.visible
                        Layout.preferredWidth: undefined // overriding default to distribute space
                        onClicked: {
                            kodi.volumeDown();
                        }
                    }

                    Slider {
                        id: volumeSlider
                        enabled: kodi.connectedHost.volumeControlType !== KodiHost.VolumeControlTypeRelative
                        visible: enabled
                        Layout.fillWidth: true
                        live: true
                        onPressedChanged: {
                            if (!pressed) {
                                kodi.volume = value
                            }
                        }
                        Binding {
                            target: volumeSlider
                            property: "value"
                            value: kodi.volume
                        }
                        Timer {
                            interval: 100
                            running: volumeSlider.pressed
                            repeat: true
                            onTriggered: {
                                kodi.volume = volumeSlider.value
                            }
                        }
                    }
                    BottomEdgeButton {
                        source: "image://theme/audio-speakers-symbolic"
                        Layout.fillWidth: !volumeSlider.visible
                        Layout.preferredWidth: undefined // overriding default to distribute space
                        onClicked: {
                            kodi.volumeUp();
                        }
                    }
                }

                SectionHeader {
                    Layout.fillWidth: true
                    color: "black"
                }
                RowLayout {
                    id: optionsRow
                    Layout.fillWidth: true
                }
            }
        }
    }
}


