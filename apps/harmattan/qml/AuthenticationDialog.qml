/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
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

import QtQuick 1.1
import com.nokia.meego 1.0
import Xbmc 1.0

Sheet {
    id: authenticationDialog
    acceptButtonText: qsTr("OK")
    rejectButtonText: qsTr("Cancel")

    property string hostname

    content: Column {
        anchors.fill: parent
        anchors.margins: appWindow.pageMargin
        spacing: 10

        Label {
            width: parent.width
            text: qsTr("XBMC on %1 requires authentication:").arg(hostname);
            wrapMode: Text.WordWrap
        }
        Label {
            text: qsTr("Username:")
        }
        TextField {
            width: parent.width
            id: username
            inputMethodHints: Qt.ImhSensitiveData|Qt.ImhNoAutoUppercase|Qt.ImhPreferLowercase|Qt.ImhUrlCharactersOnly
//            text: xbmc.username

        }
        Label {
            text: qsTr("Password:")
        }
        TextField {
            width: parent.width
            id: password
            echoMode: TextInput.PasswordEchoOnEdit
//            text: xbmc.password
        }
    }

    onAccepted: {
        console.log("sheet accepted");
        xbmc.setAuthCredentials(username.text, password.text);
        authSheet.destroy();
    }

    onRejected: {
        print("rejected");
        authSheet.destroy();
    }
}
