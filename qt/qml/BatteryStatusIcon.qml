import QtQuick

Item {
    id: batteryStatusRoot
    required property int level
    readonly property real fraction: Math.max(0, Math.min(100, level)) / 100

    DeskIcon {
        anchors.fill: parent
        glyph: "battery"
    }
    Item {
        // Clip from the cell's left edge, keeping the shell's padding empty.
        x: batteryStatusRoot.width * 5 / 24
        width: batteryStatusRoot.width * 13 / 24 * batteryStatusRoot.fraction
        height: batteryStatusRoot.height
        clip: true
        DeskIcon {
            x: -parent.x
            width: batteryStatusRoot.width
            height: batteryStatusRoot.height
            glyph: "battery_cell"
        }
    }
}
