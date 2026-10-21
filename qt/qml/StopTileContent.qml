import QtQuick

Item {
    id: content
    required property var control
    required property string ink

    Column {
        width: parent.width - 8; x: 4
        y: (parent.height - height) / 2
        spacing: 4
        Text {
            width: parent.width; text: "PARAR\nTODO"
            color: content.ink
            font.family: "Inter"; font.pixelSize: 14; font.bold: true
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.35
        }
        Text {
            width: parent.width; text: content.control.detail || ""
            color: "#ffd2cc"
            font.family: "Inter"; font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
