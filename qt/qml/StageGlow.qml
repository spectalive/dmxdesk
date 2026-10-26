import QtQuick

// Stage-light halo drawn just outside a control's edge.
Item {
    id: glow
    property color tint: "#8f7cf0"
    property real cornerRadius: 12
    property real strength: 1
    anchors.fill: parent
    z: -1
    opacity: strength
    Repeater {
        model: [{spread: 1, alpha: 0.7, line: 1}, {spread: 3, alpha: 0.32, line: 2},
                {spread: 6, alpha: 0.14, line: 3}, {spread: 10, alpha: 0.05, line: 4}]
        delegate: Rectangle {
            anchors.fill: parent
            anchors.margins: -modelData.spread
            radius: glow.cornerRadius + modelData.spread
            color: "transparent"
            border.width: modelData.line
            border.color: Qt.rgba(glow.tint.r, glow.tint.g, glow.tint.b, modelData.alpha)
        }
    }
}
