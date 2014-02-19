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
import "pages"

ApplicationWindow
{
    id: appWindow

    initialPage: mainPageComponent

    Component {
        id: mainPageComponent
        MainPage {

        }
    }

    function showAuthenticate(hostname) {
        var component = Qt.createComponent("pages/AuthenticationDialog.qml")
        if (component.status == Component.Ready) {
            var authDialog = component.createObject(initialPage);
            authDialog.hostname = hostname;
            authDialog.open();
        } else {
            console.log("Error loading component:", component.errorString());
        }
    }

    Timer {
        /*Hack, we can't use pageStack.push while the pageStack is animating*/
        id: delayedAuthenticate
        interval: 10
        repeat: true
        running: false
        onTriggered: {
            if (pageStack.busy) {
                return;
            }

            stop();
            showAuthenticate(hostname)
        }
        property string hostname
    }

    Connections {
        target: xbmc
        onAuthenticationRequired: {
            if (pageStack.busy) {
                delayedAuthenticate.hostname = hostname;
                delayedAuthenticate.start();
            } else {
                showAuthenticate(hostname);
            }
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
}
