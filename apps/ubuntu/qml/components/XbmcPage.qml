import QtQuick 2.0
import Ubuntu.Components 1.1

Page {
    id: root
    property bool replaceMode: false

    signal goToNowPlaying();
    signal goToKeypad();
    signal goToMedia();

    property alias mediaVisible: mediaAction.visible
    property alias nowPlayingVisible: nowPlayingAction.visible
    property alias keypadVisible: keypadAction.visible

    tools: ToolbarItems {
        id: mainTools
        ToolbarButton {
            action: Action {
                id: mediaAction
                text: qsTr("Media")
                iconName: "navigation-menu"
                visible: false
                onTriggered: {
                    root.goToMedia();
                }
            }
        }
        ToolbarButton {
            action: Action {
                id: nowPlayingAction
                text: qsTr("Now Playing")
                iconName: "media-playback-start"
                visible: false
                onTriggered: {
                    root.goToNowPlaying();
                }
            }
        }
        ToolbarButton {
            action: Action {
                id: keypadAction
                text: qsTr("Keypad")
                iconName: "keypad"
                visible: false
                onTriggered: {
                    root.goToKeypad();
                }
            }
        }
    }
}
