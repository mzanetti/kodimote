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
import harbour.xbmcremote 1.0

DockedPanel {
    id: panel
    property QtObject player: xbmc.activePlayer
    property bool hideTemporary: false
    property bool _opened

    open: player
    width: parent.width
    height: column.height + (2 * Theme.paddingLarge)
    contentHeight: height

    onHideTemporaryChanged: {
        if (hideTemporary) {
            _opened = open;
            hide(true);
        } else {
            if (_opened) {
                show(true);
            }
        }
    }

    Connections {
        target: Qt.inputMethod
        onVisibleChanged: {
            if (Qt.inputMethod.visible) {
                panel.hide(true);
            } else {
                panel.show(true);
            }
        }
    }

    Column {
        id: column
        width:parent.width
        height: childrenRect.height
        anchors.verticalCenter: parent.verticalCenter
        spacing: Theme.paddingMedium

        Item {
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: Theme.paddingLarge
                rightMargin: Theme.paddingLarge
            }

            height: 64

            IconButton {
                id: volumeDownButton
                height: 64
                width: height
                anchors.left: parent.left
                icon.source: "../icons/icon-m-volume-down.png"
                onClicked: xbmc.volumeDown()
            }

            Slider {
                id: volumeSlider
                anchors.left: volumeDownButton.right
                anchors.right: volumeUpButton.left
                enabled: xbmc.connectedHost.volumeControlType !== XbmcHost.VolumeControlTypeRelative
                visible: enabled
                leftMargin: Theme.paddingSmall
                rightMargin: Theme.paddingLarge

                minimumValue: 0
                maximumValue: 100

                Binding {
                    target: volumeSlider
                    property: "value"
                    value: xbmc.volume
                }
            }

            IconButton {
                id: volumeUpButton
                height: 64
                width: height
                anchors.right: parent.right
                icon.source: "image://theme/icon-m-speaker"
                onClicked: xbmc.volumeUp()
            }
        }

        PlayerControls {
            anchors.horizontalCenter: parent.horizontalCenter
            player: panel.player
        }
    }

    PushUpMenu {
        id: menu

        Row {
            spacing: Theme.itemSizeSmall
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height

            //Work around issue of the pully not being able to open
            Item {
                height: 1
                width: 1
            }

            Switch {
                icon.source: "image://theme/icon-l-shuffle"
                visible: xbmc.state == "audio"
                checked: player && player.shuffle
                onClicked: player.shuffle = ! player.shuffle
            }

            Switch {
                icon.source: player && player.repeat === Player.RepeatOne ? "../icons/icon-l-repeat-one.png" : "image://theme/icon-l-repeat"
                visible: xbmc.state == "audio"
                checked: player && player.repeat !== Player.RepeatNone
                automaticCheck: false
                onClicked: {
                    if (player.repeat === Player.RepeatNone) {
                        player.repeat = Player.RepeatOne;
                    } else if (player.repeat === Player.RepeatOne) {
                        player.repeat = Player.RepeatAll;
                    } else {
                        player.repeat = Player.RepeatNone;
                    }
                }
            }

            Switch {
                icon.source: "image://theme/icon-l-speaker"
                visible: xbmc.state == "video"
                checked: true
                automaticCheck: false
                onClicked: {
                    menu.active = false;
                    var component = Qt.createComponent("../pages/MediaSelectionDialog.qml");
                    if (component.status === Component.Ready) {;
                        var dialog = component.createObject(panel, {
                                                                mediaModel: player.audiostreams,
                                                                currentIndex: player.currentAudiostream
                                                            });
                        dialog.rejected.connect(function () {

                        });
                        pageStack.push(dialog);
                    }
                }
            }

            Switch {
                icon.source: "image://theme/icon-m-message"
                visible: xbmc.state == "video"
                checked: player && player.currentSubtitle >= 0
                automaticCheck: false
                onClicked: {
                    menu.active = false
                    var component = Qt.createComponent("../pages/MediaSelectionDialog.qml");
                    if (component.status === Component.Ready) {
                        var dialog = component.createObject(panel, {
                                                                mediaModel: player.subtitles,
                                                                currentIndex: player.currentSubtitle,
                                                                supportsOff: true
                                                            });
                        dialog.rejected.connect(function () {
                            player.currentSubtitle = -1;
                        });
                        dialog.accepted.connect(function () {
                            player.currentSubtitle = dialog.currentIndex;
                        });
                        pageStack.push(dialog);
                    }
                }
            }
        }
    }
}
