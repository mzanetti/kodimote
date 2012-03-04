import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: connectionSheet

    content: Flickable {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        Column {
            anchors.fill: parent
            spacing: 10
//            SectionHeader {
//                headerText: qsTr("Look and feel")
//                width: parent.width
//            }
            CheckBox {
                id: cbUseThumbnails
                text: qsTr("Use Thumbnails")
                checked: settings.useThumbnails
                onClicked: settings.useThumbnails = cbUseThumbnails.checked

            }
//            SectionHeader {
//                headerText: qsTr("Phone calls")
//                width: parent.width
//            }

//            CheckBox {
//                id: cbChangeVol
//                text: qsTr("Change volume")
//                checked: settings.changeVolumeOnCall
//            }
//            Slider {
//                id: slVolume
//                width: parent.width - 20
//                anchors.right: parent.right
//                value: settings.volumeOnCall
//                enabled: cbChangeVol.checked
//                maximumValue: 100
//            }
//            CheckBox {
//                id: cbPause
//                text: qsTr("Pause video")
//                checked: settings.changeVolumeOnCall
//            }
        }
    }

    tools: ToolBar {
        ToolButton {
            iconSource: "toolbar-back"
            onClicked: pageStack.pop();
        }
    }
}
