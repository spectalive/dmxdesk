import QtQuick

// Window floor: blue-black leather lit faintly from the left.
Rectangle {
    anchors.fill: parent
    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop { position: 0.0; color: "#181c26" }
        GradientStop { position: 0.45; color: "#0d1119" }
        GradientStop { position: 1.0; color: "#090c15" }
    }
    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("chrome/floor-grain.ico")
        fillMode: Image.Tile
    }
}
