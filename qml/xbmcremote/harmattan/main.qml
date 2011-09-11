import QtQuick 1.1
import com.meego 1.0

PageStackWindow {
    id: appWindow
    property int pageMargin: 16
    property bool connected: xbmc.connected
    initialPage: connected ? mainPage : noConnectionPage

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

    ToolBarLayout {
        id: toolBarEntry
        visible: false
        ToolIcon { platformIconId: "toolbar-settings";
            anchors.left: parent===undefined ? undefined : parent.left
            onClicked: myMenu.state == "open" ? myMenu.close() : myMenu.open()
        }
        ToolIcon { platformIconId: "toolbar-dialer";
            anchors.horizontalCenter: parent===undefined ? undefined : parent.horizontalCenter
            onClicked: {
                var component = Qt.createComponent("Keypad.qml")
                if (component.status == Component.Ready) {
                    pageStack.push(component);
                } else {
                    console.log("Error loading component:", component.errorString());
                }
            }
        }
        ToolIcon {
            platformIconId: "toolbar-mediacontrol-play" + (enabled ? "" : "-dimmed");
            enabled: xbmc.activePlayer !== null
            anchors.right: parent===undefined ? undefined : parent.right
            onClicked: pageStack.push(nowPlayingPage)
        }
        //        ToolIcon { platformIconId: "toolbar-view-menu";
        //             anchors.right: parent===undefined ? undefined : parent.right
        //             onClicked: {
        //             }
        //        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Connect..."
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
                text: "Invert theme"
                onClicked: {
                    theme.inverted = !theme.inverted;
                    settings.themeInverted = theme.inverted;
                }
            }
            MenuItem {
                text: "About"
                onClicked: {
                    aboutDialog.open();
                }
            }
            MenuItem {
                text: "Quit xbmc"
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

    QueryDialog {
        id: quitDialog
        message: "Do you want to quit xbmc?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        titleText: "Are you sure?"
        onAccepted:
        {
            xbmc.quit();
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
                text: "Xbmcremote 0.3.1"
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
            text: "Copyright\n Michael Zanetti\n michael_zanetti@gmx.net\n\nThanks to\n Xbmc Development Team (artwork)\n Johannes Siipola (artwork)"
          }
        }

        buttons {
            Button {
                id: closeButton
                text: "close"; onClicked: aboutDialog.close()
            }
            Button {
                anchors.top: closeButton.bottom
                anchors.topMargin: 10
                text: "donate";
                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2")
            }
        }
    }

}
