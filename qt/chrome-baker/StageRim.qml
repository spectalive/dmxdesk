import QtQuick

// Rim light: the upper edge catches a cold white light that fades down the
// sides, and the lower edge picks up the coloured stage light from below.
Item {
    id: rim
    property real cornerRadius: 12
    property color light: "#c8d4ff"
    property real strength: 1
    property color base: "transparent"
    property real baseStrength: 0
    anchors.fill: parent
    Repeater {
        model: [{reach: 0.22, alpha: 0.75}, {reach: 0.5, alpha: 0.3}]
        delegate: Item {
            width: rim.width; height: Math.ceil(rim.height * modelData.reach)
            clip: true
            Rectangle {
                width: rim.width; height: rim.height
                radius: rim.cornerRadius
                color: "transparent"
                border.width: 1
                border.color: Qt.rgba(rim.light.r, rim.light.g, rim.light.b, modelData.alpha * rim.strength)
            }
        }
    }
    Repeater {
        model: rim.baseStrength > 0 ? [{reach: 0.18, alpha: 0.9}, {reach: 0.5, alpha: 0.35}] : []
        delegate: Item {
            y: rim.height - height
            width: rim.width; height: Math.ceil(rim.height * modelData.reach)
            clip: true
            Rectangle {
                y: -(rim.height - parent.height)
                width: rim.width; height: rim.height
                radius: rim.cornerRadius
                color: "transparent"
                border.width: 1
                border.color: Qt.rgba(rim.base.r, rim.base.g, rim.base.b, modelData.alpha * rim.baseStrength)
            }
        }
    }
}
