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

Row  {
    id: header
    property string headerText
    width: parent.width
    height: headerLabel.height
    opacity: .5
    Item {
        height: parent.height
        width: 20
    }

    Rectangle {
        height: 1
        width: parent.width - headerLabel.width - 40
        anchors.verticalCenter: parent.verticalCenter
        color: headerLabel.color
    }
    Item {
        height: parent.height
        width: 20
    }
    Label {
        id: headerLabel
        text: headerText
    }
}
