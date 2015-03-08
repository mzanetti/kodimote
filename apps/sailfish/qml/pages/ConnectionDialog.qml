/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.kodimote 1.0

Dialog {
    id: connectionDialog
    canAccept: hostList.currentIndex >= 0

    onAccepted: {
        kodi.hostModel().host(hostList.currentIndex).connect();
    }

    Component {
        id: hostComponent

        KodiHost {
        }
    }

    function addHost() {
        var host = hostComponent.createObject();
        var hostPage = pageStack.push(Qt.resolvedUrl("AddHostDialog.qml"), { host: host, title: qsTr("Add") });
        hostPage.onRejected.connect(function() {
            host.destroy();
        });
        hostPage.onAccepted.connect(function() {
            kodi.hostModel().addHost(host);
        });
    }

    SilicaListView {
        id: hostList
        anchors.fill: parent
        model: kodi.hostModel()
        highlightFollowsCurrentItem: true
        currentIndex: model.indexOf(kodi.connectedHost)

        header: DialogHeader {
            id: header
            acceptText: qsTr("Connect")
            cancelText: qsTr("Cancel")
        }

        ViewPlaceholder {
            enabled: hostList.count == 0

            Column {
                spacing: Theme.paddingMedium
                visible: hostList.count == 0
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    leftMargin: Theme.paddingLarge
                    rightMargin: Theme.paddingLarge
                }

                BusyIndicator {
                    running: parent.visible
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Label {
                    font.pixelSize: Theme.fontSizeMedium
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: Theme.highlightColor
                    text: qsTr("Searching for Kodi hosts.") + "\n" + "\n"
                          + qsTr("Please enable the following options in the Services settings of Kodi:") + "\n- "
                          + qsTr("Allow control of Kodi via HTTP") + "\n- "
                          + qsTr("Allow programs on other systems to control Kodi") + "\n- "
                          + qsTr("Announce these services to other systems via Zeroconf") + "\n"
                          + qsTr("If you don't use Zeroconf, add a host manually.");
                }
            }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add host")
                onClicked: connectionDialog.addHost()
            }
        }

        delegate: ListItem {
            id: hostDelegate
            contentHeight: Theme.itemSizeMedium
            highlighted: down || menuOpen || hostList.currentIndex == index
            anchors.margins: Theme.paddingLarge

            menu: contextMenuComponent
            onClicked: {
                hostList.currentIndex = index
            }

            function wakeupHost() {
                kodi.hostModel().host(index).wakeup();
            }

            function removeHost() {
                kodi.hostModel().removeHost(index);
            }

            Label {
                text: hostname
                anchors {
                    left: parent.left
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    rightMargin: Theme.paddingLarge
                    verticalCenter: parent.verticalCenter
                }
                elide: Text.ElideRight
                font.pixelSize: Theme.fontSizeLarge
            }

            Component {
                id: contextMenuComponent
                ContextMenu {
                    MenuItem {
                        text: qsTr("Edit")
                        onClicked: {
                            var host = kodi.hostModel().host(index);
                            var hostPage = pageStack.push(Qt.resolvedUrl("AddHostDialog.qml"), { host: host, title: qsTr("Save") });
                        }
                    }
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: {
                            hostDelegate.remorseAction(qsTr("Removing %1").arg(hostname), hostDelegate.removeHost)
                        }
                    }
                    MenuItem {
                        text: qsTr("Wake up")
                        visible: hwaddr.length > 0
                        onClicked: {
                            hostDelegate.wakeupHost();
                        }
                    }
                }
            }
        }
    }

    KodiDiscovery {
        continuousDiscovery: true
    }
}
