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

Row {
    id: controlButtons

    property QtObject player

    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-previous"
        onClicked: player.skipPrevious();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-backwards"
        enabled: player && player.state == "playing" && player.type !== Player.PlayerTypePictures
        selected: player && player.speed < 0
        onClicked: player.seekBackward();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-stop"
        enabled: player && player.state == "playing"
        onClicked: player.stop();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: player && player.speed === 1 && player.state === "playing" ? "toolbar-mediacontrol-pause" : "toolbar-mediacontrol-play"
        onClicked: player.playPause();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-forward"
        enabled: player && player.state == "playing" && player.type !== Player.PlayerTypePictures
        selected: player && player.speed > 1
        onClicked: player.seekForward();
        width: parent.width / 6
    }
    MediaControlButton {
        platformIconId: "toolbar-mediacontrol-next"
        onClicked: player.skipNext();
        width: parent.width / 6
    }
}
