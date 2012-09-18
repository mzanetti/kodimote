import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
	width: 200
	height: 200
	color: "red"


    Button {
        id: button
        anchors.centerIn: parent
        //height: 44
        text: "blafdsfdsafsda"
    }

    Text {
        text: "blubb"
        anchors.top: button.bottom
    }

}
