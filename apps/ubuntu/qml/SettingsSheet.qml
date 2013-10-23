import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1

ComposerSheet {

    Component.onCompleted: {
        __foreground.__styleInstance.headerColor = "#0a2663"
        __foreground.__styleInstance.backgroundColor = "#1b62c8"
    }

    Flickable {
        height: parent.height
        width: parent.width
        contentHeight: settingsCol.height
        clip: true

        Column {
            id: settingsCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: units.gu(1)
            spacing: units.gu(1)
            SectionHeader {
                headerText: qsTr("Look and feel")
                width: parent.width
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
                }

            }

            SectionHeader {
                headerText: qsTr("Used media")
                width: parent.width
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
                }

                CheckBox {
                    id: cbVideosEnabled
                    checked: settings.videosEnabled
                }
                Label {
                    height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                    width: (parent.width - cbMusicEnabled.width * 2) / 2
                    text: qsTr("Videos")
                }

                CheckBox {
                    id: cbPicturesEnabled
                    checked: settings.picturesEnabled
                }
                Label {
                    height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                    width: (parent.width - cbMusicEnabled.width * 2) / 2
                    text: qsTr("Pictures")
                }

                CheckBox {
                    id: cbPvrEnabled
                    checked: settings.pvrEnabled
                }
                Label {
                    height: cbMusicEnabled.height; verticalAlignment: Text.AlignVCenter
                    width: (parent.width - cbMusicEnabled.width * 2) / 2
                    text: qsTr("TV Channels")
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
        }
    }

    onConfirmClicked: {
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
    }
}
