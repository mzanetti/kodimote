import QtQuick 1.0

Rectangle {
    width: parent.width
    height: 62
    property bool dragging: false

    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#000000";
        }
        GradientStop {
            position: 1.00;
            color: "#979797";
        }
    }


    Connections {
        target:  Xbmc
        onVolumeChanged: {
            if(!dragging){
                slider.x = percentToSliderPos(volume)
            }
        }
    }

    function sliderPosToPercent(x) {
        return slider.x * 100 / container.width
    }
    function percentToSliderPos(pc) {
        return pc * container.width / 100;
    }

    Rectangle {
        id: container
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 10

        height: 40
        radius: height / 2
        color: "black"
        border.color: "white"

        Rectangle {
            id: fill
            height: 40
            radius: height / 2
            color: "#0084ff"
            anchors.fill: container

            anchors.rightMargin: container.width - slider.x - slider.width
        }

        Rectangle {
            id: slider
            color: "white"
            height: 40
            width: 80
            radius: height / 2
            anchors.verticalCenter: parent.verticalCenter
            x: 1

            MouseArea {
                anchors.fill: parent
                anchors.margins: -16 // Increase mouse area a lot outside the slider
                drag.target: parent; drag.axis: Drag.XAxis
                drag.minimumX: 2; drag.maximumX: container.width - slider.width - 2

                onPressed: dragging = true
                onReleased:  dragging = false

            }
            onXChanged: {
                Xbmc.setVolume(sliderPosToPercent(x))
            }
        }
    }
//    Rectangle {
//        id: slider
//        x: 1; y: 1; width: 30; height: 14
//        radius: 6
//        smooth: true
//        gradient: Gradient {
//            GradientStop { position: 0.0; color: "#424242" }
//            GradientStop { position: 1.0; color: "black" }
//        }

//        MouseArea {
//            anchors.fill: parent
//            anchors.margins: -16 // Increase mouse area a lot outside the slider
//            drag.target: parent; drag.axis: Drag.XAxis
//            drag.minimumX: 2; drag.maximumX: container.width - 32
//        }
//    }


}
