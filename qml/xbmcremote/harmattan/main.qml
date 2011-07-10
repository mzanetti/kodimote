import QtQuick 1.1
import com.meego 1.0

PageStackWindow {
    id: appWindow
    property int pageMargin: 16
    property bool connected: xbmc.connected
    initialPage: connected ? mainPage : noConnectionPage


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
        }
    }

    onConnectedChanged: {
        console.log("connectedChanged " + connected)
        if(!connected) {
            var component = Qt.createComponent("NoConnectionPage.qml")
            if (component.status == Component.Ready) {
                pageStack.replace(component);
            } else {
                console.log("Error loading component:", component.errorString());
            }
        } else {
            var component = Qt.createComponent("MainPage.qml")
            if (component.status == Component.Ready && pageStack.busy == false) {
                pageStack.replace(component);
            } else {
                console.log("Error loading component:", component.errorString());
            }
        }
    }

}
