/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 2.0
import QtFeedback 5.0

Item {
    id: root

    property Component arrowImage: null

    property int introStep: 0

    signal enterPressed();
    signal left();
    signal right();
    signal up();
    signal down();

    Timer {
        id: teaseTimer
        interval: 1000
        running: settings.introStep < Settings.IntroStepDone
        repeat: true
        onTriggered: {
            switch (root.introStep) {
            case 0:
                leftArrows.item.animate()
                rightArrows.item.animate();
                break;
            case 1:
                upArrows.item.animate()
                downArrows.item.animate();
                break;
            case 2:
                downArrows.item.animate();
                break;
            case 3:
            case 4:
            case 5:
                animateAll();
                break;
            }
        }
    }

    function animateAll() {
        leftArrows.item.animate();
        rightArrows.item.animate();
        upArrows.item.animate();
        downArrows.item.animate();
    }

    HapticsEffect {
        id: rumbleEffect
        intensity: 0.05
        duration: 50
    }

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

                Loader {
                    id: arrowImageLoader
                    height: units.gu(3)
                    width: height
                    opacity: 0.3

                    sourceComponent: root.arrowImage

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

                        onStopped: {
                            if (index === arrowRepeater.count - 1 ) {
                                arrowsRoot.isRunning = false;
                            }
                        }

                        PauseAnimation { duration: 100 * index }
                        NumberAnimation { target: arrowImageLoader; properties: "opacity"; from: 0.3; to: 1; duration: 200 }
                        NumberAnimation { target: arrowImageLoader; properties: "opacity"; from: 1; to: 0.3; duration: 200 }
                    }
                }
            }
        }
    }

    Item {
        id: bgImage
        anchors.fill: parent

        Loader {
            id: rightArrows
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: parent.width / 4.5
            sourceComponent: arrows
        }
        Loader {
            id: leftArrows
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -parent.width / 4.5
            rotation: 180
            sourceComponent: arrows
        }
        Loader {
            id: downArrows
            anchors.centerIn: parent
            anchors.verticalCenterOffset: parent.height / 4.5
            rotation: 90
            sourceComponent: arrows
        }
        Loader {
            id: upArrows
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -parent.height / 4.5
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
            scrollTimer.start();
        }

        onReleased: {
            scrollTimer.stop();
            if (scrollTimer.triggerCount == 0) {
                doKeyPress();
            }
        }

        onPositionChanged: {
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

            property int triggerCount: 0

            onRunningChanged: {
                if (running) {
                    triggerCount = 0
                }
            }

            onTriggered: {
                triggerCount++;
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
                rumbleEffect.start(1);
                animateAll();
                root.enterPressed();
                return;
            }

            // Did we not move more than minSwipeDistance? => do nothing.
            if (dxAbs < minSwipeDistance && dyAbs < minSwipeDistance) {
                print("Only moved " + dx + "x" + dy + " pixels. Not activating gesture");
                return;
            }

            rumbleEffect.start(1);

            // if horizontal delta is larger than twice the minimum distance,
            // we always go left/right, no matter what the vertical delta is.
            // Reason is that the thumb can easily produce large vertical deltas
            // just by touching the screen with more than the tip
            if (dxAbs > minSwipeDistance * 2 || dxAbs > dyAbs) {
                if (dx < 0) {
                    leftArrows.item.animate();
                    root.left();
                } else {
                    rightArrows.item.animate();
                    root.right();
                }
            } else {
                if (dy < 0) {
                    upArrows.item.animate();
                    root.up();
                } else {
                    downArrows.item.animate();
                    root.down();
                }
            }
        }
    }
}
