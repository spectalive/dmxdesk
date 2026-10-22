import QtQuick

Item {
    id: content
    required property string label
    required property string bpm
    readonly property real bpmStretch: 1.15

    Text {
        objectName: "align-left:direct-tempo"
        x: 12; y: 11
        text: content.label
        color: "#adb6c4"
        font.family: "Inter"; font.pixelSize: 10
    }
    Item {
        id: bpmAxis
        objectName: "align-left:direct-tempo"
        x: 12; y: 22; width: bpmValue.width * content.bpmStretch; height: 70
        Text {
            id: bpmValue
            // Compensate the expanded numerals' leading ink bearing.
            x: -3; width: implicitWidth; height: parent.height
            text: content.bpm
            color: "#f5f7fa"
            font.family: "Inter"; font.pixelSize: 56; font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            transform: Scale { origin.x: 0; xScale: content.bpmStretch }
        }
    }
}
