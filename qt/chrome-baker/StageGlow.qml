import QtQuick

// Stage-light halo drawn just outside a control's edge; a strength above 1
// brightens the outer rings too.
Item {
    id: glow
    property color tint: "#8f7cf0"
    property real cornerRadius: 12
    property real strength: 1
    anchors.fill: parent
    z: -1
    Repeater {
        model: [{spread: 1, alpha: 0.7, line: 1}, {spread: 2.5, alpha: 0.4, line: 2},
                {spread: 4, alpha: 0.16, line: 2}]
        delegate: Rectangle {
            anchors.fill: parent
            anchors.margins: -modelData.spread
            radius: glow.cornerRadius + modelData.spread
            color: "transparent"
            border.width: modelData.line
            border.color: Qt.rgba(glow.tint.r, glow.tint.g, glow.tint.b,
                                  Math.min(1, modelData.alpha * glow.strength))
        }
    }
}
