import QtQuick

// Recess the control sits in: a dark bed and a soft drop shadow below its edge.
Item {
    id: well
    property real cornerRadius: 12
    property real depth: 1
    anchors.fill: parent
    z: -2
    Repeater {
        model: [{spread: 5, offset: 3, alpha: 0.18}, {spread: 3, offset: 2, alpha: 0.35}, {spread: 1.5, offset: 1, alpha: 0.6}]
        delegate: Rectangle {
            x: -modelData.spread; y: -modelData.spread + modelData.offset
            width: well.width + 2 * modelData.spread; height: well.height + 2 * modelData.spread
            radius: well.cornerRadius + modelData.spread
            color: Qt.rgba(0, 0, 0, modelData.alpha * well.depth)
        }
    }
}
