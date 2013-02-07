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

MainView {
    id: appWindow

    tools: tabs.tools

    property int pageMargins: units.gu(2)

    Rectangle {
        anchors.fill: parent
        color: "#ebebeb"

    }

    Tabs {
        id: tabs
        anchors.fill: parent
        parent: leftPane
        ItemStyle.class: "new-tabs"

        property variant tabPageItems: [ mainTab.page, nowPlayingTab.page ]

        property ActionList tools: selectedTab.page.tools

        Tab {
            id: mainTab
            title: "Media"
            page: PageStack {
                id: pageStack
                anchors.fill: parent
                property bool connected: xbmc.connected
                property bool nfcSheetOpen: false

                Component.onCompleted: pageStack.push(mainPage)

                function home() {
                    pageStack.clear();
                    pageStack.push(mainPage)
                }

                MainPage {
                    id: mainPage
                }
            }


        }

        Tab {
            id: nowPlayingTab
            title: "Now Playing"
            page: NowPlayingPage {
                id: nowPlayingPage
                anchors.fill: parent

                timerActive: tabListView.currentIndex === 1
            }
        }

        Tab {
            id: keypadTab

            title: "Keypad"

            page: Keypad {
                anchors.fill: parent
            }
        }
    }
}
