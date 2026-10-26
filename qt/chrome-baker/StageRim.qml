import QtQuick

// Rim light: the upper edge catches a cold light that fades a little way down
// to the `side` level, and the lower edge picks up the coloured stage light
// from below. Drawn as 1 px stripes so both fade inside the 9-slice caps and
// the stretched middle of the sides keeps one level.
Item {
    id: rim
    property real cornerRadius: 12
    property color light: "#c8d4ff"
    property real strength: 1
    // How much of the light stays on the sides below the top cap.
    property real side: 0
    property color base: "transparent"
    property real baseStrength: 0
    // Draws the light this far inside the edge, as a bevel instead of a line.
    property real inset: 0
    readonly property real reach: 24
    anchors.fill: parent
    function fade(distance) {
        return Math.pow(Math.max(0, 1 - distance / reach), 1.6)
    }
    Repeater {
        model: Math.ceil(rim.height)
        delegate: Item {
            required property int index
            y: index
            width: rim.width; height: 1
            clip: true
            readonly property real upper: rim.strength * (rim.side + (0.9 - rim.side) * rim.fade(index))
            readonly property real lower: rim.baseStrength * rim.fade(rim.height - 1 - index)
            visible: upper > 0.01 || lower > 0.01
            Rectangle {
                x: rim.inset; y: rim.inset - parent.index
                width: rim.width - 2 * rim.inset; height: rim.height - 2 * rim.inset
                radius: rim.cornerRadius - rim.inset
                color: "transparent"
                border.width: 1
                border.color: Qt.rgba(rim.light.r, rim.light.g, rim.light.b, parent.upper)
            }
            Rectangle {
                x: rim.inset; y: rim.inset - parent.index
                width: rim.width - 2 * rim.inset; height: rim.height - 2 * rim.inset
                radius: rim.cornerRadius - rim.inset
                color: "transparent"
                border.width: 1
                border.color: Qt.rgba(rim.base.r, rim.base.g, rim.base.b, parent.lower)
            }
        }
    }
}
