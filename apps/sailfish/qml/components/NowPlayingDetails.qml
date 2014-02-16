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

Column {
    id: column
    property QtObject player: xbmc.activePlayer
    property QtObject currentItem: player.currentItem

    ItemDetailRow {
        title: qsTr("First aired:")
        text: currentItem.firstAired
    }

    ItemDetailRow {
        title: qsTr("Genre:")
        text: currentItem.genre
    }

    ItemDetailRow {
        title: qsTr("Year:")
        text: currentItem.year
    }

    ItemDetailRow {
        title: qsTr("Director:")
        text: currentItem.director
    }

    ItemDetailRow {
        title: qsTr("Tagline:")
        text: currentItem.tagline
    }

    ItemDetailRow {
        title: qsTr("MPAA:")
        text: currentItem.mpaa
    }

    ItemDetailRow {
        title: qsTr("Instrument:")
        text: currentItem.instrument
    }

    ItemDetailRow {
        title: qsTr("Style:")
        text: currentItem.style
    }

    ItemDetailRow {
        title: qsTr("Mood:")
        text: currentItem.mood
    }

    ItemDetailRow {
        title: qsTr("Born:")
        text: currentItem.born
    }

    ItemDetailRow {
        title: qsTr("Formed:")
        text: currentItem.formed
    }

    ItemDetailRow {
        title: qsTr("Died:")
        text: currentItem.died
    }

    ItemDetailRow {
        title: qsTr("Disbanded:")
        text: currentItem.disbanded
    }

    ItemDetailRow {
        title: qsTr("Duration:")
        text: currentItem.durationString
    }

    ItemDetailRow {
        title: qsTr("Comment:")
        text: currentItem.comment
    }

    ItemDetailRow {
        title: qsTr("Playcount:")
        text: currentItem.playcount
    }

    ItemDetailRow {
        title: qsTr("Cast:")
        text: currentItem.cast
    }

    Label {
        width: parent.width
        visible: currentItem.plot.length > 0
        text: currentItem.plot;
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }
    Label {
        width: parent.width
        visible: currentItem.description.length > 0
        text: currentItem.description;
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }
}
