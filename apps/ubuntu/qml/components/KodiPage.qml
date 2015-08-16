import QtQuick 2.4
import Ubuntu.Components 1.3

Page {
    id: root

    signal goToNowPlaying();
    signal goToKeypad();
    signal goToMedia();

    property bool backVisible: false
    property alias mediaVisible: mediaAction.visible
    property alias nowPlayingVisible: nowPlayingAction.visible
    property alias keypadVisible: keypadAction.visible

    signal backPressed();

    head {
        actions: [
            Action {
                id: mediaAction
                text: qsTr("Media")
                iconName: "navigation-menu"
                visible: false
                onTriggered: {
                    root.goToMedia();
                }
            },
            Action {
                id: nowPlayingAction
                text: qsTr("Now Playing")
                iconName: "media-playback-start"
                visible: false
                onTriggered: {
                    root.goToNowPlaying();
                }
            },
            Action {
                id: keypadAction
                text: qsTr("Keypad")
                iconName: "keypad"
                visible: false
                onTriggered: {
                    root.goToKeypad();
                }
            }
        ]
        backAction: Action {
            iconName: "back"
            visible: root.backVisible && pageStack.depth > 1
            onTriggered: root.backPressed()
        }
    }
}
