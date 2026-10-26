import QtQuick

// Coloured light bleeding inward from a lit control's edge.
Item {
    id: inner
    property color tint: "#8a5cff"
    property real cornerRadius: 12
    anchors.fill: parent
    Repeater {
        model: [{inset: 1, alpha: 0.32}, {inset: 3, alpha: 0.16}, {inset: 6, alpha: 0.07}]
        delegate: Rectangle {
            anchors.fill: parent
            anchors.margins: modelData.inset
            radius: Math.max(0, inner.cornerRadius - modelData.inset)
            color: "transparent"
            border.width: modelData.inset === 1 ? 2 : 3
            border.color: Qt.rgba(inner.tint.r, inner.tint.g, inner.tint.b, modelData.alpha)
        }
    }
}
