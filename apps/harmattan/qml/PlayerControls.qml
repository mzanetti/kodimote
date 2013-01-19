import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Row {
    id: controlButtons

    property QtObject player

    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-previous"
        onClicked: player.skipPrevious();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-backwards"
        enabled: player && player.state == "playing"
        selected: player && player.speed < 0
        onClicked: player.seekBackward();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-stop"
        enabled: player && player.state == "playing"
        onClicked: player.stop();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: player && player.speed === 1 && player.state === "playing" ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play"
        onClicked: player.playPause();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-forward"
        enabled: player && player.state == "playing"
        selected: player && player.speed > 1
        onClicked: player.seekForward();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-next"
        onClicked: player.skipNext();
        width: parent.width / 6
    }
}
