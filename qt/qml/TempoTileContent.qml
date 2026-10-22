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
        x: 12; y: 19; width: bpmValue.width * content.bpmStretch; height: 50
        Text {
            id: bpmValue
            // Inter's large numerals start two rendered pixels inside the shared axis.
            x: -2; width: implicitWidth; height: parent.height
            text: content.bpm
            color: "#f5f7fa"
            font.family: "Inter"; font.pixelSize: 40; font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            transform: Scale { origin.x: 0; xScale: content.bpmStretch }
        }
    }
    Text {
        objectName: "align-left:direct-tempo"
        x: bpmAxis.x; y: 70
        width: implicitWidth
        text: "BPM BASE"
        color: "#adb6c4"
        font.family: "Inter"; font.pixelSize: 10
        horizontalAlignment: Text.AlignLeft
    }
}
