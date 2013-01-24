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

Item {
    id: appWindow

    VisualItemModel {
        id: tabModel

        PageStack {
            id: pageStack
            width: appWindow.width
            height: appWindow.height
            property int pageMargin: 16
            property bool connected: xbmc.connected
            property bool nfcSheetOpen: false

            Component.onCompleted: pageStack.push(mainPage)

            MainPage {
                id: mainPage
            }
        }


        NowPlayingPage {
            id: nowPlayingPage
            height: appWindow.height
            width: appWindow.width
            timerActive: tabListView.currentIndex === 1
        }

        Rectangle {
            height: appWindow.height
            width: appWindow.width
            color: "blue"
        }

    }

    ListView {
        id: tabListView
        orientation: ListView.Horizontal
        anchors.fill: parent
        contentHeight: appWindow.height
        contentWidth: appWindow.width
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightFollowsCurrentItem: true

        model: tabModel
    }

}
