import QtQuick

// Window floor: blue-black leather lit faintly from the left. Both layers are
// baked images; a full-window gradient Rectangle costs the tablet ~12 ms a frame.
Item {
    anchors.fill: parent
    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("chrome/floor-light.ico")
        smooth: true
    }
    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("chrome/floor-grain.ico")
        fillMode: Image.Tile
    }
}
