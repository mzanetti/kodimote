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

import QtQuick 2.4
import Ubuntu.Components 1.2

Row {
    id: row
    property int count: 5
    property int rated: 0
    Repeater {
        model: parent.rated
        Icon {
            height: row.height
            width: height
            name: "favorite-selected"
            color: "white"
        }
    }

    Repeater {
        model: parent.count - parent.rated
        Icon {
            height: row.height
            width: height
            name: "favorite-unselected"
            color: "white"
        }
    }
}
