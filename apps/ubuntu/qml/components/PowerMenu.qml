import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: powerMenu
    title: qsTr("Power menu")

    signal selectUser()

    Column {
        anchors { left: parent.left; right: parent.right }
        spacing: units.gu(1)

        Button {
            text: qsTr("Change user")
            width: parent.width
            iconName: "contact"
            visible: kodi.profiles().count > 1
            onClicked: {
                powerMenu.selectUser();
                PopupUtils.close(powerMenu)
            }
        }

        Button {
            text: qsTr("Disconnect")
            width: parent.width
            iconName: "close"
            onClicked: {
                kodi.disconnectFromHost();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Quit Kodi")
            width: parent.width
            iconName: "system-log-out"
            onClicked: {
                kodi.quit();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Shutdown Kodi host")
            width: parent.width
            iconName: "system-shutdown"
            visible: kodi.canShutdown
            onClicked: {
                kodi.quit();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Reboot Kodi host")
            iconName: "system-restart"
            width: parent.width
            visible: kodi.canReboot
            onClicked: {
                kodi.reboot();
                PopupUtils.close(powerMenu)
            }
        }
        Button {
            text: qsTr("Suspend Kodi host")
            width: parent.width
            iconName: "torch-off"
            visible: kodi.canSuspend
            onClicked: {
                PopupUtils.close(powerMenu)
                kodi.suspend();
            }
        }
        Button {
            text: qsTr("Hibernate Kodi host")
            width: parent.width
            visible: kodi.canHibernate
            iconName: "save"
            onClicked: {
                PopupUtils.close(powerMenu)
                kodi.hibernate();
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
