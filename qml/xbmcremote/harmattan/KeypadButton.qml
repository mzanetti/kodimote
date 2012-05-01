// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
//import QtMobility.feedback 1.1

Image {
    id: button
    property string icon
    signal pressed()
    source: "icons/" + button.icon + ".png"

    property int mouseAreaLeftMargin: 0
    property int mouseAreaRightMargin: 0
    property int mouseAreaTopMargin: 0
    property int mouseAreaBottomMargin: 0

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        anchors.leftMargin: button.mouseAreaLeftMargin
        anchors.rightMargin: button.mouseAreaRightMargin
        anchors.topMargin: button.mouseAreaTopMargin
        anchors.bottomMargin: button.mouseAreaBottomMargin
        onPressed: rumbleEffect.start();
        onClicked:button.pressed();
    }
    states: [
        State {
            when: mouseArea.pressed;
            PropertyChanges { target: button; source: "icons/" + button.icon + "_pressed.png" }
        }
    ]

//    HapticsEffect {
//        id: rumbleEffect
//        attackIntensity: 0.0
//        attackTime: 250
//        intensity: 1.0
//        duration: 100
//        fadeTime: 250
//        fadeIntensity: 0.0
//    }
}
