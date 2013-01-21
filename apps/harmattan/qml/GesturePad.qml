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
                if (!isRunning) {
                    startAnimation();
                } else {
                    onceMore = true;
                }
            }

            signal startAnimation()
            property bool isRunning: false
            property bool onceMore: false

            onIsRunningChanged: {
                if (!isRunning && onceMore) {
                    onceMore = false;
                    startAnimation();
                }
            }

            Repeater {
                id: arrowRepeater
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

                        onStarted: {
                            if (index === 0 ) {
                                arrowsRoot.isRunning = true;
                            }
                        }

                        onCompleted: {
                            if (index === arrowRepeater.count - 1 ) {
                                arrowsRoot.isRunning = false;
                            }
                        }

                        PauseAnimation { duration: 100 * index }
                        NumberAnimation { target: arrowImage; properties: "opacity"; from: 0.3; to: 1; duration: 200 }
                        NumberAnimation { target: arrowImage; properties: "opacity"; from: 1; to: 0.3; duration: 200 }
                    }
                }
            }
        }
    }

    Item {
        id: bgImage
        anchors.centerIn: parent
        width: parent.width
        height: width * 0.75

        Rectangle {
            anchors.fill: parent
            opacity: theme.inverted ? 0.1 : 0.05
            radius: 20
            color: theme.inverted ? "white" : "black"
        }

        Image {
            anchors.fill: parent
            source: "icons/pad-separator.png"
        }

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
        id: mouseArea
        anchors.fill: parent

        property int startx
        property int starty
        property int maxClickDistance: 5
        property int minSwipeDistance: 100

        onPressed: {
            startx = mouse.x
            starty = mouse.y
        }

        onPressAndHold: {
            scrollTimer.start();
        }

        onReleased: {
            if (scrollTimer.running) {
                scrollTimer.stop();
            } else {
                doKeyPress();
            }
        }

        onMousePositionChanged: {
            if (scrollTimer.running) {
                var dxAbs = Math.abs(mouseX - startx);
                var dyAbs = Math.abs(mouseY - starty)

                if (dxAbs > dyAbs) {
                    scrollTimer.newSpeed = Math.min(100, Math.max(0, 100 * (dxAbs - minSwipeDistance) / (mouseArea.width - minSwipeDistance)));
                } else {
                    scrollTimer.newSpeed = Math.min(100, Math.max(0, 100 * (dyAbs - minSwipeDistance) / (mouseArea.height - minSwipeDistance)));
                }
            }
        }

        Timer {
            id: scrollTimer
            running: false
            repeat: true
            interval: maxInterval - ((maxInterval - minInterval) * speed / 100);

            property int minInterval: 50
            property int maxInterval: 500
            // Percentage
            property int speed: 0

            // Timer restarts on interval change which blocks it on frequent changes
            // Lets use newSpeed for changing and fetch it when appropriate
            property int newSpeed: -1

            onTriggered: {
                if(newSpeed !== -1) {
                    speed = newSpeed;
                    newSpeed = -1;
                }
                mouseArea.doKeyPress();
            }
        }

        function doKeyPress() {
            var dx = mouseX - startx;
            var dy = mouseY - starty;
            var dxAbs = Math.abs(dx);
            var dyAbs = Math.abs(dy)

            // Did we not move? => press enter
            if (dxAbs < maxClickDistance && dyAbs < maxClickDistance) {
                print("pressing enter")
                keys.select();
                return;
            }

            // Did we not move more than minSwipeDistance? => do nothing.
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
