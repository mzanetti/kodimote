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
    Component.onCompleted: {
        console.log("settings: " + settings);
        console.log("use thumbnail: " + settings.useThumbnails);
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: settingsCol.childrenRect.height

        Column {
            id: settingsCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingSmall

            DialogHeader {
                id: header
                acceptText: qsTr("Save")
            }

            SectionHeader {
                text: qsTr("Look and feel")
            }

            TextSwitch {
                id: useThumbnails
                text: qsTr("Use Thumbnails")
                checked: settings.useThumbnails
            }
            /*TextSwitch {
                id: keepDisplayLit
                text: qsTr("Keep display on when charging")
                checked: settings.keepDisplayLit
            }*/
            TextSwitch {
                id: ignoreArticle
                text: qsTr("Ignore articles for sorting")
                checked: settings.ignoreArticle
            }

            SectionHeader {
                text: qsTr("Used media")
            }

            Row {
                width: parent.width
                TextSwitch {
                    id: musicEnabled
                    text: qsTr("Music")
                    checked: settings.musicEnabled
                    width: parent.width / 2
                }
                TextSwitch {
                    id: videosEnabled
                    text: qsTr("Videos")
                    checked: settings.videosEnabled
                    width: parent.width / 2
                }
            }

            Row {
                width: parent.width
                TextSwitch {
                    id: picturesEnabled
                    text: qsTr("Pictures")
                    checked: settings.picturesEnabled
                    width: parent.width / 2
                }
                TextSwitch {
                    id: pvrEnabled
                    text: qsTr("Live TV")
                    checked: settings.pvrEnabled
                    width: parent.width / 2
                    visible: xbmc.pvrAvailable
                }
            }

            SectionHeader {
                text: qsTr("Phone calls")
            }

            TextSwitch {
                id: changeVol
                text: qsTr("Change volume")
                checked: settings.changeVolumeOnCall
            }
            Slider {
                id: volume
                width: parent.width - 20
                anchors.right: parent.right
                value: settings.volumeOnCall
                enabled: changeVol.checked
                maximumValue: 100
                valueText: enabled ? value : ""
                stepSize: 1
            }
            TextSwitch {
                id: pauseVideo
                text: qsTr("Pause video")
                checked: settings.pauseVideoOnCall
            }
            TextSwitch {
                id: pauseMusic
                text: qsTr("Pause music")
                checked: settings.pauseMusicOnCall
            }
            TextSwitch {
                id: showNotifications
                text: qsTr("Show call notifications")
                checked: settings.showCallNotifications
            }
        }
    }

    onAccepted: {
        //settings.keepDisplayLit = keepDisplayLit.checked
        settings.ignoreArticle = ignoreArticle.checked
        settings.changeVolumeOnCall = changeVol.checked
        settings.volumeOnCall = volume.value
        settings.pauseMusicOnCall = pauseMusic.checked
        settings.pauseVideoOnCall = pauseVideo.checked
        settings.useThumbnails = useThumbnails.checked
        settings.showCallNotifications = showNotifications.checked
        settings.musicEnabled = musicEnabled.checked
        settings.videosEnabled = videosEnabled.checked
        settings.picturesEnabled = picturesEnabled.checked
        settings.pvrEnabled = pvrEnabled.checked
    }
}
