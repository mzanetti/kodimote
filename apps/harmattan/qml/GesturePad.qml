import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

Item {
    id: root
    height: bgImage.height
    width: parent.width

    Component {
        id: arrows
        Row {
            id: arrowsRoot

            function animate() {
                startAnimation();
            }

            signal startAnimation()

            Repeater {
                model: 3
                Image {
                    id: arrowImage
                    source: "image://theme/icon-m-toolbar-next-white"
                    opacity: 0.3

                    Connections {
                        target: arrowsRoot
                        onStartAnimation: {
                            animation.start();
                        }
                    }

                    SequentialAnimation {
                        id: animation
                        PauseAnimation { duration: 150 * index }
                        NumberAnimation { target: arrowImage; properties: "opacity"; from: 0.3; to: 1; duration: 300 }
                        NumberAnimation { target: arrowImage; properties: "opacity"; from: 1; to: 0.3; duration: 300 }
                    }
                }
            }
        }
    }

    Image {
        id: bgImage
        anchors.centerIn: parent
        source: "icons/pad-bg.png"

        Loader {
            id: rightArrows
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: parent.width / 4
            sourceComponent: arrows
        }
        Loader {
            id: leftArrows
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -parent.width / 4
            rotation: 180
            sourceComponent: arrows
        }
        Loader {
            id: downArrows
            anchors.centerIn: parent
            anchors.verticalCenterOffset: parent.height / 4
            rotation: 90
            sourceComponent: arrows
        }
        Loader {
            id: upArrows
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -parent.height / 4
            rotation: -90
            sourceComponent: arrows
        }

    }

    MouseArea {
        anchors.fill: parent

        property int startx
        property int starty
        property int maxClickDistance: 5
        property int minSwipeDistance: 100

        onPressed: {
            startx = mouse.x
            starty = mouse.y
        }

        onReleased: {
            var dx = mouse.x - startx;
            var dy = mouse.y - starty;
            var dxAbs = Math.abs(dx);
            var dyAbs = Math.abs(dy)

            // Did we not move?
            if (dxAbs < maxClickDistance && dyAbs < maxClickDistance) {
                keys.select();
                return;
            }

            // Did we move far enough?
            if (dxAbs < minSwipeDistance && dyAbs < minSwipeDistance) {
                print("Only moved " + dx + "x" + dy + " pixels. Not activating gesture");
                return;
            }

            // if horizontal delta is larger than twice the minimum distance,
            // we always go left/right, no matter what the vertical delta is.
            // Reason is that the thumb can easily produce large vertical deltas
            // just by touching the screen with more than the tip
            if (dxAbs > minSwipeDistance * 2 || dxAbs > dyAbs) {
                if (dx < 0) {
                    leftArrows.item.animate();
                    keys.left();
                } else {
                    rightArrows.item.animate();
                    keys.right();
                }
            } else {
                if (dy < 0) {
                    upArrows.item.animate();
                    keys.up();
                } else {
                    downArrows.item.animate();
                    keys.down();
                }
            }
        }
    }
}
