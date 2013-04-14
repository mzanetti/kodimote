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
import org.kde.plasma.components 0.1
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: root
    property int spacing
    TabBar {
        id: tabbar
        anchors {
            left: parent.left
            right: parent.right
        }
        TabButton { tab: nowPlaying; text: i18n("Now Playing")}
        TabButton { tab: library; text: i18n("Library")}

    }

    TabGroup {
        anchors {
            top: tabbar.bottom
            topMargin: root.spacing
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        NowPlaying {
            id: nowPlaying
            spacing: root.spacing
            height: root.height
            width: root.width
        }
        Browser {
            id: library
            height: root.height
            width: root.width
            spacing: root.spacing
        }

    }

//    VisualItemModel {
//        id: tabModel
//        NowPlaying {
//            id: nowPlaying
//            spacing: root.spacing
//            height: view.height
//            width: view.width
//        }
//        Rectangle {
//            height: view.height
//            width: view.width
//            color: "blue"
//        }
//    }
//    ListView {
//        id: view
//        anchors.fill: parent
//        model: tabModel
//    }
}
