import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: connectionSheet

    Flickable {
        height: parent.height
        width: parent.width
        contentHeight: settingsCol.height
        Column {
            id: settingsCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: appWindow.pageMargin
            spacing: 10
            SectionHeader {
                headerText: qsTr("Look and feel")
                width: parent.width
            }

            CheckBox {
                id: cbUseThumbnails
                text: qsTr("Use Thumbnails")
                checked: settings.useThumbnails
            }
            CheckBox {
                id: cbKeepDisplayLit
                text: qsTr("Keep display on when charging")
                checked: settings.keepDisplayLit
            }
            CheckBox {
                id: cbIgnoreArticle
                text: qsTr("Ignore articles for sorting")
                checked: settings.ignoreArticle
            }

            SectionHeader {
                headerText: qsTr("Phone calls")
                width: parent.width
            }

            CheckBox {
                id: cbChangeVol
                text: qsTr("Change volume")
                checked: settings.changeVolumeOnCall
            }
            Slider {
                id: slVolume
                width: parent.width - 20
                anchors.right: parent.right
                value: settings.volumeOnCall
                enabled: cbChangeVol.checked
                maximumValue: 100
            }
            CheckBox {
                id: cbPauseVideo
                text: qsTr("Pause video")
                checked: settings.pauseVideoOnCall
            }
            CheckBox {
                id: cbPauseMusic
                text: qsTr("Pause music")
                checked: settings.pauseMusicOnCall
            }
            CheckBox {
                id: cbShowNotifications
                text: qsTr("Show call notifications")
                checked: settings.showCallNotifications
            }
        }
    }

    tools: ToolBarLayout {
        id: toolBarEntry
        visible: false
        ToolButton { iconSource: "toolbar-back";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: {
                console.log("sheet accepted")
                settings.keepDisplayLit = cbKeepDisplayLit.checked
                settings.ignoreArticle = cbIgnoreArticle.checked
                settings.changeVolumeOnCall = cbChangeVol.checked
                settings.volumeOnCall = slVolume.value
                settings.pauseMusicOnCall = cbPauseMusic.checked
                settings.pauseVideoOnCall = cbPauseVideo.checked
                settings.useThumbnails = cbUseThumbnails.checked
                settings.showCallNotifications = cbShowNotifications.checked
                pageStack.pop();
            }
        }
    }

}
