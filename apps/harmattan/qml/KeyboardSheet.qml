import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

Sheet {
    id: root
    property alias titleText: title.text
    property string type

    property string inputedText
    property string initialValue

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

        Item {
            id: time
            visible: false
            anchors {
                top: title.bottom
                left: parent.left
                right: parent.right
            }

            TimePicker {
                id: timePicker
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

                function orientationSuffix() {
                    if (screen.currentOrientation === Screen.Portrait || screen.currentOrientation === Screen.PortraitInverted )
                        return "portrait"
                    else
                        return "landscape"
                }

                backgroundImage: "image://theme/meegotouch-timepicker-light-1-" + orientationSuffix()
                hourDotImage: "image://theme/meegotouch-timepicker-disc-hours-" + orientationSuffix()
                minutesDotImage: "image://theme/meegotouch-timepicker-disc-minutes-" + orientationSuffix()

                onHoursChanged: {
                    root.inputedText = xbmc.keys().formatTime(hours, minutes);
                }

                onMinutesChanged: {
                    root.inputedText = xbmc.keys().formatTime(hours, minutes);
                }
            }
        }

        Item {
            id: date
            visible: false
            anchors {
                top: title.bottom
                left: parent.left
                right: parent.right
            }

            DatePicker {
                id: datePicker

                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }

                function orientationString() {
                    if (screen.currentOrientation === Screen.Portrait || screen.currentOrientation === Screen.PortraitInverted )
                        return "portrait"
                    else
                        return "landscape"
                }

                platformStyle: DatePickerStyle {
                    orientationString: datePicker.orientationString()
                }

                onSelectedDateChanged: {
                    root.inputedText = Qt.formatDate(selectedDate, "dd-MM-yyyy");
                }
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
                text: initialValue
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
                text: initialValue
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
            name: "time"
            when: type == "time"
            PropertyChanges {
                target: time
                visible: true
            }
            PropertyChanges {
                target: timePicker
                hours: 0
                minutes: 0
            }

            StateChangeScript {
                script: {
                    var colonIndex = initialValue.indexOf(':');
                    timePicker.hours = parseInt(initialValue.substring(0, colonIndex));
                    timePicker.minutes = parseInt(initialValue.substring(colonIndex+1));
                }
            }
        },
        State {
            name: "date"
            when: type == "date"
            PropertyChanges {
                target: date
                visible: true
            }

            StateChangeScript {
                script: {
                    var dayMonthSeperator = initialValue.indexOf('/');
                    var monthYearSeperator = initialValue.substring(dayMonthSeperator+1).indexOf('/', dayMonthSeperator) + dayMonthSeperator+1;
                    var day = parseInt(initialValue.substring(0, dayMonthSeperator));
                    var month = parseInt(initialValue.substring(dayMonthSeperator+1, monthYearSeperator));
                    var year = parseInt(initialValue.substring(monthYearSeperator+1));
                    var date = new Date(year, month - 1, day);
                    datePicker.setDate(date);
                }
            }
        }

    ]

    onAccepted: {
        xbmc.keys().sendText(inputedText);
    }
}
