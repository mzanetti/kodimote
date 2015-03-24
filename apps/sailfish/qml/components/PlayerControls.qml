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
import QtFeedback 5.0
import harbour.kodimote 1.0

Row {
    id: playerControls

    property QtObject player
    spacing: Theme.paddingSmall

    HapticsEffect {
        id: rumbleEffect
        intensity: 0.50
        duration: 50
    }

    IconButton {
        icon.source: "image://theme/icon-m-previous"
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: !!player
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.skipPrevious()
        }
    }

    IconButton {
        icon.source: "../icons/icon-m-backwards.png"
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.seekBackward()
        }
        highlighted: down || (playerControls.player && playerControls.player.speed < 0)
    }

    IconButton {
        icon.source: "../icons/icon-m-stop.png"
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: player ? player.state !== "stopped" : false
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.stop()
        }
    }

    IconButton {
        icon.source: "image://theme/icon-m-" + (player && player.speed === 1 && player.state === "playing" ? "pause" : "play")
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: !!player
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.playPause()
        }
    }

    IconButton {
        icon.source: "../icons/icon-m-forward.png"
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: player ? player.state == "playing" && player.type !== Player.PlayerTypePictures : false
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.seekForward()
        }
        highlighted: down || (playerControls.player && playerControls.player.speed > 1)
    }

    IconButton {
        icon.source: "image://theme/icon-m-next"
        icon.height: 75; icon.width: 75
        height: 75; width: 75
        enabled: !!player
        onClicked: {
            if (settings.hapticsEnabled) {
                rumbleEffect.start(2);
            }
            playerControls.player.skipNext()
        }
    }
}
