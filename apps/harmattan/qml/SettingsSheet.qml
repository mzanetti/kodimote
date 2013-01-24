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

Sheet {
    id: connectionSheet
    acceptButtonText: qsTr("Save")
    rejectButtonText: qsTr("Cancel")

    content: Flickable {
        height: parent.height
        width: parent.width
        contentHeight: settingsCol.height
        Column {
            id: settingsCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: appWindow.pageMargin
            spacing: 10
            SectionHeader {
                headerText: qsTr("Look and feel")
                width: parent.width
            }

            CheckBox {
                id: cbInvertTheme
                text: qsTr("Invert theme")
                checked: settings.themeInverted
            }
            CheckBox {
                id: cbUseThumbnails
                text: qsTr("Use Thumbnails")
                checked: settings.useThumbnails
            }
            CheckBox {
                id: cbKeepDisplayLit
                text: qsTr("Keep display on when charging")
                checked: settings.keepDisplayLit
            }
            CheckBox {
                id: cbIgnoreArticle
                text: qsTr("Ignore articles for sorting")
                checked: settings.ignoreArticle
            }

            SectionHeader {
                headerText: qsTr("Phone calls")
                width: parent.width
            }

            CheckBox {
                id: cbChangeVol
                text: qsTr("Change volume")
                checked: settings.changeVolumeOnCall
            }
            Slider {
                id: slVolume
                width: parent.width - 20
                anchors.right: parent.right
                value: settings.volumeOnCall
                enabled: cbChangeVol.checked
                maximumValue: 100
            }
            CheckBox {
                id: cbPauseVideo
                text: qsTr("Pause video")
                checked: settings.pauseVideoOnCall
            }
            CheckBox {
                id: cbPauseMusic
                text: qsTr("Pause music")
                checked: settings.pauseMusicOnCall
            }
            CheckBox {
                id: cbShowNotifications
                text: qsTr("Show call notifications")
                checked: settings.showCallNotifications
            }
        }
    }

    onAccepted: {
        console.log("sheet accepted")
        theme.inverted = cbInvertTheme.checked;
        settings.themeInverted = cbInvertTheme.checked
        settings.keepDisplayLit = cbKeepDisplayLit.checked
        settings.ignoreArticle = cbIgnoreArticle.checked
        settings.changeVolumeOnCall = cbChangeVol.checked
        settings.volumeOnCall = slVolume.value
        settings.pauseMusicOnCall = cbPauseMusic.checked
        settings.pauseVideoOnCall = cbPauseVideo.checked
        settings.useThumbnails = cbUseThumbnails.checked
        settings.showCallNotifications = cbShowNotifications.checked
    }
}
