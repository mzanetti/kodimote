import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: "Save"
    rejectButtonText: "Cancel"

    content: Column {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        spacing: 10
        CheckBox {
            id: cbInvertTheme
            text: "Invert theme"
            checked: settings.themeInverted
        }
        CheckBox {
            id: cbChangeVol
            text: "Change volume during calls"
            checked: settings.changeVolumeOnCall
        }
        Slider {
            id: slVolume
            width: parent.width
            value: settings.volumeOnCall
            enabled: cbChangeVol.checked
            maximumValue: 100
        }
        CheckBox {
            id: cbPause
            text: "Pause video during calls"
            checked: settings.changeVolumeOnCall
        }
    }

    onAccepted: {
        console.log("sheet accepted")
        theme.inverted = cbInvertTheme.checked;
        settings.themeInverted = cbInvertTheme.checked
        settings.changeVolumeOnCall = cbChangeVol.checked
        settings.volumeOnCall = slVolume.value
        settings.pauseOnCall = cbPause.checked
    }
}
