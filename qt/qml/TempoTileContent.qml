import QtQuick

Item {
    id: content
    required property string label
    required property string bpm

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
        x: 12; y: 19; width: bpmValue.width; height: 50
        Text {
            id: bpmValue
            objectName: "align-center:direct-tempo-bpm"
            // Inter's large numerals start two rendered pixels inside the shared axis.
            x: -2; width: implicitWidth; height: parent.height
            text: content.bpm
            color: "#f5f7fa"
            font.family: "Inter"; font.pixelSize: 40; font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
    Text {
        objectName: "align-center:direct-tempo-bpm"
        x: bpmAxis.x + bpmValue.x + (bpmValue.width - width) / 2; y: 70
        width: Math.max(implicitWidth, bpmValue.width)
        text: "BPM BASE"
        color: "#adb6c4"
        font.family: "Inter"; font.pixelSize: 10
        horizontalAlignment: Text.AlignHCenter
    }
}
