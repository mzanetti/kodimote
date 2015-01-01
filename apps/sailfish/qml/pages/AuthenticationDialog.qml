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

Dialog {
    id: authenticationDialog

    property string hostname

    DialogHeader {
        id: header
        acceptText: qsTr("Authenticate")
    }

    Column {
        anchors {
            top: header.bottom;
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right;
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Theme.paddingLarge
            text: qsTr("XBMC on %1 requires authentication:").arg(hostname);
            wrapMode: Text.WordWrap
            font.family: Theme.fontFamilyHeading
            color: Theme.highlightColor
        }

        TextField {
            id: username
            width: parent.width
            placeholderText: qsTr("Username")
            label: qsTr("Username")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: password.focus = true
        }

        TextField {
            id: password
            width: parent.width
            placeholderText: qsTr("Password")
            label: qsTr("Password")

            EnterKey.enabled: text.length > 0
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: authenticationDialog.accept()
        }
    }

    onAccepted: {
        kodi.setAuthCredentials(username.text, password.text);
    }
}
