import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: qsTr("Save")
    rejectButtonText: qsTr("Cancel")

    content: Flickable {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        Column {
            anchors.fill: parent
            spacing: 10
            SectionHeader {
                headerText: qsTr("Look and feel")
                width: parent.width
            }

            CheckBox {
                id: cbInvertTheme
                text: qsTr("Invert theme")
                checked: settings.themeInverted
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
        }
    }

    onAccepted: {
        console.log("sheet accepted")
        theme.inverted = cbInvertTheme.checked;
        settings.themeInverted = cbInvertTheme.checked
        settings.changeVolumeOnCall = cbChangeVol.checked
        settings.volumeOnCall = slVolume.value
        settings.pauseMusicOnCall = cbPauseMusic.checked
        settings.pauseVideoOnCall = cbPauseVideo.checked
        settings.useThumbnails = cbUseThumbnails.checked
        settings.keepDisplayLit = cbKeepDisplayLit.checked
    }
}
