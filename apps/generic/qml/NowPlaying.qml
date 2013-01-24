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

FocusScope {
    id: nowPlayling
    anchors.fill: parent

    property QtObject player: xbmc.activePlayer
    property QtObject playlist: player.playlist()
    property QtObject currentItem: playlist.currentItem

    state: "nowPlaying"

    NowPlayingView {
        id: nowPlayingView
        anchors.fill: parent
        state: "hidden"
        focus: true
        onOpenPlaylist: {
            console.log("clicked3")
            nowPlayling.state = "playlist"
            audioPlaylist.forceActiveFocus();
        }
    }

    AudioPlaylist {
        id: audioPlaylist
        anchors.fill: parent
        model: playlist
        state: "hidden"
        onClosePlaylist: {
            nowPlayling.state = "nowPlaying"
            nowPlayingView.forceActiveFocus();
        }
    }

    states: [
        State {
            name: "nowPlaying"
            PropertyChanges { target: nowPlayingView; state: "visible" }
            PropertyChanges { target: audioPlaylist; state: "hidden" }
        },
        State {
            name: "playlist"
            PropertyChanges { target: nowPlayingView; state: "hidden" }
            PropertyChanges { target: audioPlaylist; state: "visible" }
        }

    ]
}
