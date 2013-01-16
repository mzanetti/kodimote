import QtQuick 1.1
import com.nokia.meego 1.0
import xbmcremote 1.0

Item {
    id: root
    height: bgImage.height
    width: parent.width

    Image {
        id: bgImage
        anchors.centerIn: parent
        source: "icons/pad-bg.png"
    }

    MouseArea {
        anchors.fill: parent

        property int startx
        property int starty
        property int minSwipeDistance: 50

        onPressed: {
            startx = mouse.x
            starty = mouse.y
        }

        onReleased: {
            var dx = mouse.x - startx;
            var dy = mouse.y - starty;

            // Did we move far enough?
            if (Math.abs(dx) < minSwipeDistance && Math.abs(dy) < minSwipeDistance) {
                print("Only moved " + dx + "x" + dy + " pixels. Not activating gesture");
                return;
            }

            if (Math.abs(dx) > Math.abs(dy)) {
                if (dx < 0) {
                    keys.left();
                } else {
                    keys.right();
                }
            } else {
                if (dy < 0) {
                    keys.up();
                } else {
                    keys.down();
                }
            }
        }
    }
}
