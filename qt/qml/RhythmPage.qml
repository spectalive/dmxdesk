import QtQuick

Item {
    required property var dials
    x: 0; y: 0; width: 888; height: 600

    Repeater {
        model: parent.dials
        delegate: Rectangle {
            id: card
            property var dial: modelData
            x: 16 + index * 418; y: 56; width: 410; height: 456; radius: 16
            color: "#14181e"; border.color: "#39404d"
            Text { x: 16; y: 16; text: modelData.caption; color: "#adb6c4"; font.pixelSize: 12; font.bold: true }
            Text {
                x: 16; y: 63; width: 240; text: modelData.known ? modelData.bpm : "--"
                color: "#f5f7fa"; font.pixelSize: 52; font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Text { x: 16; y: 129; width: 240; text: "BPM BASE"; color: "#adb6c4"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter }
            Rectangle {
                x: 274; y: 52; width: 120; height: 48; radius: 10
                color: "#262c36"; border.color: "#39404d"
                Text { anchors.centerIn: parent; text: "Tiempo ×" + (card.dial.factorName || "?"); color: "#f5f7fa"; font.pixelSize: 13 }
            }
            Repeater {
                model: [
                    {label:"TAP", x:16, y:168, w:378, h:96},
                    {label:"−1 BPM", x:16, y:276, w:185, h:72},
                    {label:"+1 BPM", x:209, y:276, w:185, h:72},
                    {label:"MÁS LENTO ×2", x:16, y:360, w:185, h:72},
                    {label:"MÁS RÁPIDO ×2", x:209, y:360, w:185, h:72}
                ]
                delegate: Rectangle {
                    x: modelData.x; y: modelData.y; width: modelData.w; height: modelData.h
                    radius: 12; color: "#1b1f27"; border.color: "#39404d"
                    opacity: card.dial.enabled && !card.dial.pending ? 1 : 0.65
                    Text {
                        anchors.centerIn: parent; text: modelData.label
                        color: "#f5f7fa"; font.pixelSize: 16; font.bold: true
                    }
                }
            }
            Text {
                x: 16; y: 148; width: 370; text: card.dial.note || ""
                color: "#adb6c4"; font.pixelSize: 11; elide: Text.ElideRight
            }
        }
    }
    Rectangle {
        x: 16; y: 520; width: 828; height: 64; radius: 12
        color: "#1b1f27"; border.color: "#39404d"
        Text { anchors.centerIn: parent; text: "TAP AMBOS"; color: "#f5f7fa"; font.pixelSize: 16; font.bold: true }
    }
}
