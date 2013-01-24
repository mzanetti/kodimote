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

Dialog {
    id: writeNfcTagSheet
    //acceptButtonText: "Close"
    property string errorMessage

    Component.onCompleted: {
        if(writeNfcTagSheet.errorMessage.length == 0) {
            nfcHandler.writeTag();
        }
    }

    Component.onDestruction: {
        print("nfc sheet destroyed");
        appWindow.nfcSheetOpen = false;
        nfcHandler.cancelWriteTag();
    }

    content: Label {
        height: 200
        width: 400
        anchors.centerIn: parent
        id: textLabel
        text: writeNfcTagSheet.errorMessage.length == 0 ? qsTr("Tap a NFC tag to write XBMC connection information to it. You can then use the tag to connect to this XBMC.") : writeNfcTagSheet.errorMessage
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        font.pointSize: 25
    }

    Connections {
        target: nfcHandler

        onTagWritten: {
            textLabel.text = statusText
            closeButton.text = qsTr("Close")
        }
    }

    onRejected: {
        console.log("sheet accepted")
        writeNfcTagSheet.deleteLater();
    }

    buttons {
        Button {
            id: closeButton
            text: writeNfcTagSheet.errorMessage.length == 0 ? qsTr("Cancel") : qsTr("Write NFC Tag");
            onClicked: {
                if(writeNfcTagSheet.errorMessage.length == 0) {
                    writeNfcTagSheet.reject();
                } else {
                    writeNfcTagSheet.errorMessage = "";
                    nfcHandler.writeTag();
                }
            }
        }
    }
}
