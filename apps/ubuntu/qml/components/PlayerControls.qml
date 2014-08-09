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

    height: button.height

    property QtObject player
    spacing: (width - (button.width * 6)) / 5

    MediaControlButton {
        id: button
        source: "image://theme/media-skip-backward"
        onClicked: player.skipPrevious();
    }
    MediaControlButton {
        source: "image://theme/media-seek-backward"
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        selected: player ? player.speed < 0 : false
        onClicked: player.seekBackward();
    }
    MediaControlButton {
        source: "image://theme/media-playback-stop"
        enabled: player ? player.state == "playing" : false
        onClicked: player.stop();
    }
    MediaControlButton {
        source: "image://theme/media-playback-" + ((player && player.speed === 1 && player.state === "playing") ? "pause" : "start")
        onClicked: player.playPause();
    }
    MediaControlButton {
        source: "image://theme/media-seek-forward"
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        selected: player ? player.speed > 1 : false
        onClicked: player.seekForward();
    }
    MediaControlButton {
        source: "image://theme/media-skip-forward"
        onClicked: player.skipNext();
    }
}


