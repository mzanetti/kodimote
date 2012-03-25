import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

PageStackWindow {
    id: appWindow
    property int pageMargin: 16
    property bool connected: xbmc.connected
    property bool nfcSheetOpen: false
    initialPage: connected ? mainPage : noConnectionPage
    showStatusBar: pageStack.currentPage.height > pageStack.currentPage.width ? true : false

    Component.onCompleted: {
        theme.inverted = settings.themeInverted
    }

    MainPage{
        id: mainPage
        // model: mainMenuModel
    }

    NoConnectionPage {
        id: noConnectionPage
    }

    NowPlayingPage {
        id: nowPlayingPage
    }

    Keypad {
        id: keypadPage
    }

    PictureControlsPage {
        id: pictureControlsPage
    }

    ToolBarLayout {
        id: toolBarEntry
        visible: false
        ToolIcon { platformIconId: "toolbar-settings";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: (myMenu.status == DialogStatus.Closed || myMenu.status == DialogStatus.Closing) ? myMenu.open() : myMenu.close()
        }
        ToolIcon { platformIconId: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                if(xbmc.picturePlayerActive) {
                    pageStack.push(pictureControlsPage);
                } else {
                    pageStack.push(keypadPage);
                }

            }
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.push(nowPlayingPage);
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Connect...")
                onClicked: {
                    var component = Qt.createComponent("ConnectionSheet.qml")
                    if (component.status == Component.Ready) {
                        component.createObject(myMenu).open()
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }
            MenuItem {
                text: qsTr("Write NFC Tag")
                onClicked: {
                    var component = Qt.createComponent("WriteNfcTagSheet.qml")
                    if (component.status == Component.Ready) {
                        component.createObject(myMenu).open();
                    } else {
                        console.log("Error loading component:", component.errorString());
                    }
                }
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    onClicked: {
                        var component = Qt.createComponent("SettingsSheet.qml")
                        if (component.status == Component.Ready) {
                            component.createObject(myMenu).open()
                        } else {
                            console.log("Error loading component:", component.errorString());
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("About")
                onClicked: {
                    aboutDialog.open();
                }
            }
            MenuItem {
                text: qsTr("Quit xbmc")
                onClicked: {
                    quitDialog.open();
                }
            }
        }
    }

    onConnectedChanged: {
        console.log("connectedChanged " + connected)
        pageStack.clear();
        if(!connected) {
            var component = Qt.createComponent("NoConnectionPage.qml")
            if (component.status == Component.Ready) {
                pageStack.push(component);
            } else {
                console.log("Error loading component:", component.errorString());
            }
        } else {
            var component = Qt.createComponent("MainPage.qml")
            if (component.status == Component.Ready) {
                pageStack.push(component);
            } else {
                console.log("Error loading component:", component.errorString());
            }
        }
    }

    Connections {
        target: xbmc
        onAuthenticationRequired: {
            print("auth required");
            var component = Qt.createComponent("AuthenticationSheet.qml")
            if (component.status == Component.Ready) {
                var authSheet = component.createObject(mainPage);
                authSheet.hostname = hostname;
                authSheet.open();
            } else {
                console.log("Error loading component:", component.errorString());
            }
        }
    }

    Connections {
        target: nfcHandler
        onTagError: {
            if(!appWindow.nfcSheetOpen) {
                var component = Qt.createComponent("WriteNfcTagSheet.qml")
                if (component.status === Component.Ready) {
                    var sheet = component.createObject(myMenu, { errorMessage: errorMessage });
                    sheet.open();
                    appWindow.nfcSheetOpen = true;
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
        }
    }

    Dialog {
        id: quitDialog
        buttons {
            Column {
                spacing: 30
                Column {
                    spacing: 10
                    Button {
                        text: qsTr("Quit")
                        onClicked: xbmc.quit();
                    }
                    Button {
                        text: qsTr("Shutdown")
                        visible: xbmc.canShutdown
                        onClicked: xbmc.shutdown();
                    }
                    Button {
                        text: qsTr("Reboot")
                        visible: xbmc.canReboot
                        onClicked: xbmc.reboot();
                    }
                    Button {
                        text: qsTr("Suspend")
                        visible: xbmc.canSuspend
                        onClicked: xbmc.suspend();
                    }
                    Button {
                        text: qsTr("Hibernate")
                        visible: xbmc.canHibernate
                        onClicked: xbmc.hibernate();
                    }
                }

                Button {
                    text: qsTr("Cancel")
                    onClicked: quitDialog.close();
                }
            }
        }
    }

    Dialog {
        id: aboutDialog

        title: Item {
            height: 200
            width: parent.width
            Image {
                id: icon
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                source: "icons/xbmcremote_harmattan.png"
            }
            Text {
                anchors.top: icon.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
                color: "white"
                font.pixelSize: 32
                text: "Xbmcremote 1.0"
            }

        }

        content:Item {
            id: name
            height: 200
            width: parent.width
            Label {
                id: text
                font.pixelSize: 22
                anchors.centerIn: parent
                color: "white"
                text: qsTr("Copyright") + "\n Michael Zanetti\n michael_zanetti@gmx.net\n\n"
            }
        }

        buttons {
            Button {
                id: closeButton
                text: qsTr("Close")
                onClicked: aboutDialog.close()
            }
            Button {
                id: donateButton
                anchors.top: closeButton.bottom
                anchors.topMargin: 10
                text: qsTr("Donate")
                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2")
            }
            Button {
                anchors.top: donateButton.bottom
                anchors.topMargin: 10
                text: qsTr("Flattr")
                onClicked: Qt.openUrlExternally("http://flattr.com/thing/412274/Xbmcremote")
            }
        }
    }
}
