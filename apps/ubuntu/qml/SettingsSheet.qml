import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.0
import Ubuntu.Components.ListItems 1.0
import "components"

Dialog {
    id: root
    title: qsTr("Media browser settings")

    Column {
        id: settingsCol
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: units.gu(1)

        SectionHeader {
            headerText: qsTr("Look and feel")
            width: parent.width
            color: "black"
        }

        //            Standard {
        //                id: cbKeepDisplayLit
        //                text: qsTr("Keep display on when charging")
        //                control: Switch {
        //                    checked: settings.keepDisplayLit
        //                }
        //            }

        Row {
            width: parent.width
            spacing: units.gu(1)
            CheckBox {
                id: cbIgnoreArticle
                checked: settings.ignoreArticle
            }
            Label {
                width: parent.width - x
                text: qsTr("Ignore articles for sorting")
                anchors.verticalCenter: parent.verticalCenter
                color: "black"
            }

        }

        SectionHeader {
            headerText: qsTr("Used media")
            width: parent.width
            color: "black"
        }

        Grid {
            width: parent.width
            columns: 4
            height: childrenRect.height
            spacing: units.gu(1)
            CheckBox {
                id: cbMusicEnabled
                checked: settings.musicEnabled
            }
            Label {
                height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                width: (parent.width - cbMusicEnabled.width * 2) / 2
                text: qsTr("Music")
                color: "black"
            }

            CheckBox {
                id: cbVideosEnabled
                checked: settings.videosEnabled
            }
            Label {
                height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                width: (parent.width - cbMusicEnabled.width * 2) / 2
                text: qsTr("Videos")
                color: "black"
            }

            CheckBox {
                id: cbPicturesEnabled
                checked: settings.picturesEnabled
            }
            Label {
                height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                width: (parent.width - cbMusicEnabled.width * 2) / 2
                text: qsTr("Pictures")
                color: "black"
            }

            CheckBox {
                id: cbPvrEnabled
                checked: settings.pvrEnabled
            }
            Label {
                height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                width: (parent.width - cbMusicEnabled.width * 2) / 2
                text: qsTr("TV")
                color: "black"
            }
        }


        //            SectionHeader {
        //                headerText: qsTr("Phone calls")
        //                width: parent.width
        //            }

        //            Standard {
        //                id: cbChangeVol
        //                text: qsTr("Change volume")
        //                control: Switch {
        //                    checked: settings.changeVolumeOnCall
        //                }
        //            }

        //            Slider {
        //                id: slVolume
        //                width: parent.width - 20
        //                anchors.right: parent.right
        //                value: settings.volumeOnCall
        //                enabled: cbChangeVol.checked
        //                maximumValue: 100
        //            }

        //            Standard {
        //                id: cbPauseVideo
        //                text: qsTr("Pause video")
        //                control: Switch {
        //                    checked: settings.pauseVideoOnCall
        //                }
        //            }

        //            Standard {
        //                id: cbPauseMusic
        //                text: qsTr("Pause music")
        //                control: Switch {
        //                    checked: settings.pauseMusicOnCall
        //                }
        //            }

        //            Standard {
        //                id: cbShowNotifications
        //                text: qsTr("Show call notifications")
        //                control: Switch {
        //                    checked: settings.showCallNotifications
        //                }
        //            }

        Button {
            text: qsTr("Close")
            width: parent.width
            color: UbuntuColors.green

            onClicked: {
                console.log("sheet accepted")
                //        settings.keepDisplayLit = cbKeepDisplayLit.checked
                settings.ignoreArticle = cbIgnoreArticle.checked
                //        settings.changeVolumeOnCall = cbChangeVol.checked
                //        settings.volumeOnCall = slVolume.value
                //        settings.pauseMusicOnCall = cbPauseMusic.checked
                //        settings.pauseVideoOnCall = cbPauseVideo.checked
                //        settings.showCallNotifications = cbShowNotifications.checked
                settings.musicEnabled = cbMusicEnabled.checked
                settings.videosEnabled = cbVideosEnabled.checked
                settings.picturesEnabled = cbPicturesEnabled.checked
                settings.pvrEnabled = cbPvrEnabled.checked
                PopupUtils.close(root)
            }
        }
    }
}
