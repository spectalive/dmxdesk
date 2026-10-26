import QtQuick

Item {
    required property var dials
    x: 0; y: 0; width: 888; height: 600

    Repeater {
        model: parent.dials
        delegate: StagePanel {
            id: card
            property var dial: modelData
            x: 16 + index * 418; y: 56; width: 410; height: 456
            Text {
                objectName: "align-center:rhythm-" + index
                x: 16; y: 16; width: 240
                text: modelData.caption; color: "#adb6c4"
                font.pixelSize: 12; font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                objectName: "align-center:rhythm-" + index
                x: 16; y: 63; width: 240; text: modelData.known ? modelData.bpm : "--"
                color: "#f5f7fa"; font.pixelSize: 52; font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
            Rectangle {
                x: 274; y: 52; width: 120; height: 48; radius: 10
                color: "#151a28"; border.color: "#3a4775"
                StageWell { cornerRadius: parent.radius }
                StageSheen { cornerRadius: parent.radius }
                StageRim { cornerRadius: parent.radius }
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
                    radius: 12; color: "#1a1e33"
                    border.color: modelData.label === "TAP" ? "#6c5ce8" : "#3a4775"
                    opacity: card.dial.enabled && !card.dial.pending ? 1 : 0.65
                    StageWell { cornerRadius: parent.radius }
                    StageGlow { visible: modelData.label === "TAP"; cornerRadius: parent.radius; tint: "#7c6cf0"; strength: 0.45 }
                    StageSheen { cornerRadius: parent.radius }
                    StageRim { cornerRadius: parent.radius }
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
        color: "#1a1e33"; border.color: "#6c5ce8"
        StageWell { cornerRadius: parent.radius }
        StageGlow { cornerRadius: parent.radius; tint: "#7c6cf0"; strength: 0.45 }
        StageSheen { cornerRadius: parent.radius }
        StageRim { cornerRadius: parent.radius }
        Text { anchors.centerIn: parent; text: "TAP AMBOS"; color: "#f5f7fa"; font.pixelSize: 16; font.bold: true }
    }
}
