import QtQuick

// Window floor: the concept's leather and stage light, exported as one image
// by qt/export_stage_floor.py; a full-window gradient Rectangle costs the
// tablet ~12 ms a frame.
Image {
    anchors.fill: parent
    source: Qt.resolvedUrl("chrome/floor.bmp")
    smooth: true
}
