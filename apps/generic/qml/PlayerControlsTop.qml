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

import Qt 4.7
import Xbmc 1.0

Row {
    id: playerControlsTop
    anchors.fill: parent
    property bool volumeVisible: false

    signal playlistButtonClicked()

//    Item {
//        height: parent.height
//        width: parent.width / 5

//        Image {
//            source: "images/OSDPresetSettingsNF.png"
//            anchors.centerIn: parent
//        }

//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//            }
//        }
//    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            anchors.centerIn: parent
            source: xbmc.activePlayer.repeat === Player.RepeatNone ? "images/OSDRepeatNF.png" : (xbmc.activePlayer.repeat === Player.RepeatOne ? "images/OSDRepeatOneFO.png" : "images/OSDRepeatAllFO.png")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(xbmc.activePlayer.repeat === Player.RepeatNone) {
                    xbmc.activePlayer.repeat = Player.RepeatOne;
                } else if(xbmc.activePlayer.repeat === Player.RepeatOne) {
                    xbmc.activePlayer.repeat = Player.RepeatAll;
                } else {
                    xbmc.activePlayer.repeat = Player.RepeatNone;
                }
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: xbmc.activePlayer.shuffle ? "images/OSDRandomOffFO.png" : "images/OSDRandomOffNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                xbmc.activePlayer.shuffle = ! xbmc.activePlayer.shuffle
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: "images/OSDPlaylistNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("clicked1");
                playerControlsTop.playlistButtonClicked();
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 4

        Image {
            source: volumeBar.state == "volumeVisible" ? "images/OSDAudioFO.png" : "images/OSDAudioNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(volumeBar.state == "volumeVisible") {
                    volumeBar.state = "none"
                } else {
                    volumeBar.state = "volumeVisible"
                }
            }
        }
    }
}

