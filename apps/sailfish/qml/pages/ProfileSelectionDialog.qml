/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014-2015 Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Kodimote                                             *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,               *
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
    onAccepted: {
        listView.model.switchProfile(listView.currentIndex)
    }

    Connections {
        target: listView.model
        onCurrentProfileIndexChanged: {
            if (listView.currentIndex === -1) {
                listView.currentIndex = listView.model.currentProfileIndex;
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: listView.model.busy
        visible: listView.model.busy
        size: BusyIndicatorSize.Large
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: kodi.profiles();
        currentIndex: model.currentProfileIndex

        header: DialogHeader {
            acceptText: qsTr("Select user")
        }

        delegate: ListItem {
            width: listView.width
            height: Theme.itemSizeLarge
            contentHeight: height

            highlighted: down || listView.currentIndex === index

            onClicked: {
                listView.currentIndex = index
            }

            Label {
                text: title
                anchors.margins: Theme.paddingLarge
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                font.weight: Font.Bold
                font.pixelSize: Theme.fontSizeMedium
                elide: Text.ElideRight
            }
        }
    }
}
