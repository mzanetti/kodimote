import Qt 4.7

Item {
    id: bar
    anchors.left: parent.left
    anchors.right: parent.right
    height: 40
    property bool dragging: false

    function sliderPosToPercent(x) {
        return slider.x * 100 / container.width
    }
    function percentToSliderPos(pc) {
        return pc * bar.width / 100;
    }

    Rectangle {
        id: container
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        height: 40
        width: parent.width
        radius: height / 2
        color: "#1d1d1d"
        border.color: "#333333"
        border.width: 2

        Rectangle {
            id: fill
            height: 40
            radius: height / 2
            color: "#8fb2c2"
            border.color: "#c7d9e2"
            border.width: 2
            anchors.fill: container

            anchors.rightMargin: container.width - slider.x - slider.width
        }

        Rectangle {
            id: slider
            color: "#c7d9e2"
            border.color: "#c7d9e2"
            border.width: 2
            height: 40
            width: 80
            radius: height / 2
            anchors.verticalCenter: parent.verticalCenter
            x: percentToSliderPos(Xbmc.volume)


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
}
