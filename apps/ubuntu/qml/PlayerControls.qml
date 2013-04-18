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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Xbmc 1.0

Row {
    id: controlButtons

    property QtObject player

    MediaControlButton {
        source: "media-skip-backward"
        onClicked: player.skipPrevious();
        width: parent.width / 6
    }
    MediaControlButton {
        source: "toolbar-mediacontrol-backwards"
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        selected: player ? player.speed < 0 : false
        onClicked: player.seekBackward();
        width: parent.width / 6
    }
    MediaControlButton {
        source: "media-playback-stop"
        enabled: player ? player.state == "playing" : false
        onClicked: player.stop();
        width: parent.width / 6
    }
    MediaControlButton {
        source: (player && player.speed === 1 && player.state === "playing") ? "media-playback-pause" : "media-playback-start"
        onClicked: player.playPause();
        width: parent.width / 6
    }
    MediaControlButton {
        source: "toolbar-mediacontrol-forward"
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        selected: player ? player.speed > 1 : false
        onClicked: player.seekForward();
        width: parent.width / 6
    }
    MediaControlButton {
        source: "media-skip-forward"
        onClicked: player.skipNext();
        width: parent.width / 6
    }
}
