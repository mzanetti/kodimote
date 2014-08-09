import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0

Dialog {
    id: powerMenu
    title: qsTr("Power menu")

    Column {
        anchors { left: parent.left; right: parent.right }
        spacing: units.gu(1)

        Button {
            text: qsTr("Disconnect")
            width: parent.width
            iconName: "close"
            onClicked: {
                xbmc.disconnectFromHost();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Quit XBMC")
            width: parent.width
            iconName: "system-log-out"
            onClicked: {
                xbmc.quit();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Shutdown XBMC host")
            width: parent.width
            iconName: "system-shutdown"
            visible: xbmc.canShutdown
            onClicked: {
                xbmc.quit();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Reboot XBMC host")
            iconName: "system-restart"
            width: parent.width
            visible: xbmc.canReboot
            onClicked: {
                xbmc.reboot();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Suspend XBMC host")
            width: parent.width
            iconName: "torch-off"
            visible: xbmc.canSuspend
            onClicked: {
                PopupUtils.close(powerMenu)
                xbmc.suspend();
            }
        }
        Button {
            text: qsTr("Hibernate XBMC host")
            width: parent.width
            visible: xbmc.canHibernate
            iconName: "save"
            onClicked: {
                PopupUtils.close(powerMenu)
                xbmc.hibernate();
            }
        }
        Button {
            text: qsTr("Cancel")
            width: parent.width
            iconName: "cancel"
            onClicked: {
                PopupUtils.close(powerMenu)
            }
        }
    }
}
