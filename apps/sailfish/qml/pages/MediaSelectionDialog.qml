/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
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

Dialog {
    id: mediaSelectionDialog

    property alias mediaModel: mediaSelection.model
    property int currentIndex
    property bool supportsOff: false

    SilicaListView {
        id: mediaSelection

        header: DialogHeader {
            cancelText: mediaSelectionDialog.supportsOff ? qsTr("Off") : undefined;
        }

        anchors.fill: parent

        delegate: ListItem {
            height: Theme.itemSizeLarge
            width: parent.width
            highlighted: down || index === mediaSelectionDialog.currentIndex

            onClicked: {
                mediaSelectionDialog.currentIndex = index;
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                    verticalCenter: parent.verticalCenter
                }
                text: modelData
            }
        }
    }
}
