/*****************************************************************************
 * Copyright: 2015 Michael Zanetti <michael_zanetti@gmx.net>                 *
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

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.0

Dialog {
    id: root
    title: item.title
    property variant item

    signal accepted();
    signal rejected();

    Button {
        text: qsTr("Resume at %1").arg(item.resumeString)
        color: UbuntuColors.green
        onClicked: {
            root.accepted();
            PopupUtils.close(root)
        }
    }
    Button {
        text: qsTr("Start from beginning")
        color: UbuntuColors.red
        onClicked: {
            root.rejected();
            PopupUtils.close(root)
        }
    }
}
