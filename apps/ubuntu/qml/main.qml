import QtQuick 2.0
import Ubuntu.Components 0.1

PageStack {
    id: appWindow
    property int pageMargin: 16
    property bool connected: xbmc.connected
    property bool nfcSheetOpen: false

    Component.onCompleted: appWindow.push(mainPage)


//    Component.onCompleted: {
//        print("connecting", xbmc.connecting, "connected", xbmc.connected)
//        if(!(xbmc.connecting || xbmc.connected)) {
//            var component = Qt.createComponent("ConnectionSheet.qml")
//            if (component.status == Component.Ready) {
//                component.createObject(mainPage).open()
//            } else {
//                console.log("Error loading component:", component.errorString());
//            }
//        }
//    }

    MainPage {
        id: mainPage
    }



}
