// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Loader {
    width: 100
    height: 100

    sourceComponent: artworkSource.length > 0 ? artworkItem : defaultTextItem

    property string artworkSource
    property string defaultText
    property string testColor: "green"
    property variant fillMode: Image.Stretch
    property bool smooth: false

//    Component {
//        id: testItem
//        Rectangle {
//            color: testColor
//            anchors.fill: parent
//        }
//    }

    Component {
        id: artworkItem
        Image {
            source: artworkSource
            anchors.fill: parent
            sourceSize.height: parent.fillMode === Image.Stretch ? height : undefined
            sourceSize.width: parent.fillMode === Image.Stretch ? width : undefined
            fillMode: Image.PreserveAspectFit
            smooth: parent.smooth
        }
    }

    Component {
        id: defaultTextItem
        Text {
            text: "<b>" + defaultText + "</b> " + defaultText + " " + defaultText
            wrapMode: Text.WrapAnywhere
            anchors.fill: parent
            font.pointSize: height / 6
            clip: true
            property int colorNr: Math.floor(Math.random() * 5)
            color:  colorNr == 0  ? "lightblue" : colorNr == 1 ? "red" : colorNr == 2 ? "lightgreen" : colorNr == 3 ? "yellow" : "white"
        }
    }

}
