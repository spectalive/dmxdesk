import QtQuick

// Recess the control sits in: a dark bed and a soft drop shadow below its edge.
Item {
    id: well
    property real cornerRadius: 12
    property real depth: 1
    property real frame: 0
    anchors.fill: parent
    z: -2
    Rectangle {
        visible: well.frame > 0
        x: -4; y: -4; width: well.width + 8; height: well.height + 8
        radius: well.cornerRadius + 4
        color: Qt.rgba(0, 0, 0, well.frame)
    }
    Repeater {
        model: [{spread: 3, offset: 1, alpha: 0.25}, {spread: 2, offset: 1, alpha: 0.4}, {spread: 1, offset: 0.5, alpha: 0.6}]
        delegate: Rectangle {
            x: -modelData.spread; y: -modelData.spread + modelData.offset
            width: well.width + 2 * modelData.spread; height: well.height + 2 * modelData.spread
            radius: well.cornerRadius + modelData.spread
            color: Qt.rgba(0, 0, 0, modelData.alpha * well.depth)
        }
    }
}
