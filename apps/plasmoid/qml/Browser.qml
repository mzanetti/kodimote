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
import Xbmc 1.0
import Separator 0.1

Row {
    id: root

    function resetToModel(model) {
        while (pagestack.depth > 1) {
            var page = pagestack.pop();
            page.destroy(); // plasma bug?
        }
        browserPage.model = model;
    }

    ListModel {
        id: mainMenuModel

        // Hack: Its' not possible to call a function in ListElements :/
        Component.onCompleted: {
            append({ iconSource: "audio-headphones", targetModel: xbmc.audioLibrary() });
            append({ iconSource: "media-optical-blu-ray", targetModel: xbmc.videoLibrary() })
            append({ iconSource: "camera-photo", targetModel: xbmc.shares("pictures") })
            append({ iconSource: "video-television", targetModel: xbmc.channelGroups() })
            append({ iconSource: "document-open-folder", targetModel: xbmc.shares("") })
            browserPage.model = mainMenuModel.get(0).targetModel;
        }
    }

    Column {
        spacing: root.spacing

        Repeater {
            model: mainMenuModel
            delegate: MediaControlButton {
                source: model.iconSource
                onClicked: {
                    root.resetToModel(model.targetModel)
                }
            }
        }
    }

    Separator {
        orientation: Qt.Vertical
        height: parent.height
    }

    PageStack {
        id: pagestack
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: (parent.width - x) - column3.width - parent.spacing * 2 - separator.width

        initialPage: browserPage

        BrowserPage {
            id: browserPage
            anchors.fill: parent
            spacing: root.spacing
        }
    }

    Separator {
        id: separator
        orientation: Qt.Vertical
        height: parent.height
    }

    Item {
        id: column3
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: height
        height: parent.height

        Component {
            id: libraryButtons
            Column {
                spacing: root.spacing
                Button {
                    text: qsTr("Update library")
                    width: parent.width
                    onClicked: browserPage.model.scanForContent();
                }
                Button {
                    text: qsTr("Clean library")
                    width: parent.width
                    onClicked: browserPage.model.clean();
                }
            }
        }
        Component {
            id: itemDetails
            Rectangle {
                color: "black"
                Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    smooth: false
                    source: pagestack.currentPage.model.get(pagestack.currentPage.currentIndex, "thumbnail")
                    sourceSize.height: height

                    Rectangle {
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                            //margins: root.spacing / 2
                        }
                        //radius: root.spacing / 2
                        color: Qt.rgba(0, 0, 0, 0.6)
                        height: childrenRect.height

                        Column {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            Label {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    margins: root.spacing / 2
                                }
                                horizontalAlignment: Text.AlignHCenter
                                elide: Text.ElideRight
                                text: pagestack.currentPage.model.get(pagestack.currentPage.currentIndex, "title")
                                visible: text.length > 0
                            }
                            Label {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    margins: root.spacing / 2
                                }
                                horizontalAlignment: Text.AlignHCenter
                                elide: Text.ElideRight
                                text: pagestack.currentPage.model.get(pagestack.currentPage.currentIndex, "subtitle")
                                visible: text.length > 0
                            }
                        }
                    }
                }
            }

        }

        Loader {
            anchors.fill: parent

            sourceComponent: {
                var currentModel = pagestack.currentPage.model;
                if (currentModel.parentModel() == null) {
                    var audioLib = mainMenuModel.get(0).targetModel;
                    var videoLib = mainMenuModel.get(1).targetModel;
                    if (currentModel == audioLib || currentModel == videoLib) {
                        return libraryButtons;
                    } else {
                        return undefined;
                    }
                } else {
                    return itemDetails;
                }
            }
        }
    }
}

