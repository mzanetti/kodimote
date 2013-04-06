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
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.graphicslayouts 4.7 as GraphicsLayouts

ListView {
    height: theme.iconSizes.dialog
    orientation: ListView.Horizontal
    spacing: (width - (theme.iconSizes.dialog * 6)) / 5
    interactive: false

    property variant dataSource

    model: ListModel {
        ListElement { iconSource: "media-skip-backward"; operation: "SkipPrevious" }
        ListElement { iconSource: "media-seek-backward"; operation: "SeekBackward" }
        ListElement { iconSource: "media-playback-stop"; operation: "Stop" }
        ListElement { iconSource: "media-playback-start"; iconSource2: "media-playback-pause"; state: "playing" ; operation: "PlayPause" }
        ListElement { iconSource: "media-seek-forward"; operation: "SeekForward" }
        ListElement { iconSource: "media-skip-forward"; operation: "SkipNext" }
    }


    delegate: PlasmaCore.IconItem {
        width: theme.iconSizes.dialog
        height: width
        source: /*iconSource2 !== undefined && */dataSource.data['Player'].state == model.state ? iconSource2 : iconSource
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var data = dataSource.serviceForSource('Player').operationDescription(operation);
                dataSource.serviceForSource('Player').startOperationCall(data);
            }
        }
    }
}
