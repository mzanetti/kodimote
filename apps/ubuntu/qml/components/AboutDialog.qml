import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: aboutDialog
    title: "        Kodimote 2.1.2.1"
    text: "Michael Zanetti<br>michael_zanetti@gmx.net" +
          "<br>Robert Meijers<br>robert.meijers@gmail.com" +
          "<br>http://notyetthere.org/ubuntu/kodimote"


    Item {
        width: parent.width
        height: units.gu(34)
        MouseArea {
            anchors { left: parent.left; right: parent.right; bottom: parent.top; bottomMargin: units.gu(2) }
            height: units.gu(4)
            onClicked: Qt.openUrlExternally("http://notyetthere.org/ubuntu/kodimote")
        }

        UbuntuShape {
            anchors.left: parent.left
            anchors.bottom: parent.top
            anchors.bottomMargin: units.gu(15.5)
            height: units.gu(6)
            width: units.gu(6)
            radius: "medium"
            source: Image {
                source: "../images/kodimote.svg"
            }
        }
        Column {
            id: contentColumn
            anchors.fill: parent
            spacing: units.gu(1)


            Flickable {
                width: parent.width
                height: parent.height - y - (closeButton.height + parent.spacing) * 3
                contentHeight: gplLabel.implicitHeight
                clip: true
                Label {
                    id: gplLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: "This program is free software: you can redistribute it and/or modify " +
                          "it under the terms of the GNU General Public License as published by " +
                          "the Free Software Foundation, either version 3 of the License, or " +
                          "(at your option) any later version.\n\n" +

                          "This program is distributed in the hope that it will be useful, " +
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of " +
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the " +
                          "GNU General Public License for more details.\n\n" +

                          "You should have received a copy of the GNU General Public License " +
                          "along with this program.  If not, see http://www.gnu.org/licenses/."
                }
            }
            Button {
                id: donateButton
                width: parent.width
                text: "Donate";
                color: UbuntuColors.green
                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CWFYRZH8XNYF2")
            }
            Button {
                text: "flattr";
                color: UbuntuColors.green
                width: parent.width
                onClicked: Qt.openUrlExternally("http://flattr.com/thing/412274/Kodimote")
            }
            Button {
                id: closeButton
                width: parent.width
                text: qsTr("Close")
                color: UbuntuColors.red
                onClicked: PopupUtils.close(aboutDialog)
            }
        }
    }
}
