/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodiremote                                           *
 *                                                                           *
 * Kodiremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodiremote is distributed in the hope that it will be useful,             *
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

Page {
    SilicaListView {
        id: listView
        anchors.fill: parent
        model: xbmcMenuModel

        header: PageHeader {
            title: qsTr("XBMC on %1").arg(xbmc.connectedHostName)
        }

        delegate: ListItem {
            id: listItem

            contentHeight: Theme.itemSizeExtraLarge

            onClicked: xbmcMenuModel.click(index)

            Image {
                id: img
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter

                source: icon
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: img.right
                anchors.right: parent.right
                anchors.leftMargin: 14

                Label {
                    id: mainText
                    text: listView.model.title(index)
                    font.weight: Font.Bold
                    font.pixelSize: Theme.fontSizeLarge
                }
            }
        }

        ListModel {
            id: xbmcMenuModelTemplate
            ListElement {
                icon: "image://theme/icon-l-dismiss"
                target: "quit"
            }
            ListElement {
                icon: "image://theme/icon-l-power"
                target: "shutdown"
            }
            ListElement {
                icon: "../icons/icon-l-reboot.png"
                target: "reboot"
            }
            ListElement {
                icon: "../icons/icon-l-suspend.png"
                target: "suspend"
            }
            ListElement {
                icon: "../icons/icon-l-hibernate.png"
                target: "hibernate"
            }
        }

        ListModel {
            id: xbmcMenuModel
            // workaround: its not possible to have qsTr() in ListElements for now...
            function title(index) {
                var item = xbmcMenuModel.get(index);

                if (item) {
                    var target = xbmcMenuModel.get(index).target;
                    if (target === "quit") {
                        return qsTr("Quit");
                    }
                    if (target === "shutdown") {
                        return qsTr("Shutdown");
                    }
                    if (target === "reboot") {
                        return qsTr("Reboot");
                    }
                    if (target === "suspend") {
                        return qsTr("Suspend");
                    }
                    if (target === "hibernate") {
                        return qsTr("Hibernate");
                    }
                }
                return "";
            }

            function click(index) {
                var item = xbmcMenuModel.get(index);

                if (!item) {
                    return;
                }

                var target = xbmcMenuModel.get(index).target;
                if (target === "quit") {
                    xbmc.quit();
                }
                else if (target === "shutdown") {
                    xbmc.shutdown();
                }
                else if (target === "reboot") {
                    xbmc.reboot();
                }
                else if (target === "suspend") {
                    xbmc.suspend();
                }
                else if (target === "hibernate") {
                    xbmc.hibernate();
                }
            }
        }
    }

    function populateKodiMenu() {
        xbmcMenuModel.clear();
        xbmcMenuModel.append(xbmcMenuModelTemplate.get(0));
        if (xbmc.canShutdown) {
            xbmcMenuModel.append(xbmcMenuModelTemplate.get(1));
        }
        if (xbmc.canReboot) {
            xbmcMenuModel.append(xbmcMenuModelTemplate.get(2));
        }
        if (xbmc.canShutdown) {
            xbmcMenuModel.append(xbmcMenuModelTemplate.get(3));
        }
        if (xbmc.canHibernate) {
            xbmcMenuModel.append(xbmcMenuModelTemplate.get(4));
        }
    }

    Component.onCompleted: {
        populateKodiMenu();
    }

    Connections {
        target: xbmc
        onSystemPropertiesChanged: populateKodiMenu();
    }
}
