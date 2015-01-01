/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
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

import Qt 4.7

Row {
    property QtObject player: kodi.activePlayer
    property QtObject playlist: player.playlist()

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDPrevTrackNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.skipPrevious()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: (player.speed < 1 && player.speed !== 0 ) ? "images/OSDRewindFO.png" : "images/OSDRewindNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.seekBackward()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: (player.speed !== 1 || player.state !== "playing") ? "images/OSDPlayNF.png" : "images/OSDPauseNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.playPause()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDStopNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.stop()
            }
        }
    }

    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: player.speed > 1 ? "images/OSDForwardFO.png" : "images/OSDForwardNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.seekForward()
            }
        }
    }
    Item {
        height: parent.height
        width: parent.width / 6

        Image {
            source: "images/OSDNextTrackNF.png"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                player.skipNext()
            }
        }
    }
    states: [
        State {
            name: "stopped"
        },
        State {
            name: "playing"
        },
        State {
            name: "paused"
        }

    ]
}
