import QtQuick

// Stage-light halo drawn just outside a control's edge.
Item {
    id: glow
    property color tint: "#8f7cf0"
    property real cornerRadius: 12
    property real strength: 1
    anchors.fill: parent
    opacity: strength
    Repeater {
        model: [{spread: 1, alpha: 0.55}, {spread: 3, alpha: 0.22}, {spread: 6, alpha: 0.09}]
        delegate: Rectangle {
            anchors.fill: parent
            anchors.margins: -modelData.spread
            radius: glow.cornerRadius + modelData.spread
            color: "transparent"
            border.width: modelData.spread === 1 ? 1 : 2
            border.color: Qt.rgba(glow.tint.r, glow.tint.g, glow.tint.b, modelData.alpha)
        }
    }
}
