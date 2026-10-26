import QtQuick

Item {
    id: content
    required property var control
    required property string ink

    Column {
        objectName: "align-x-parent"
        width: parent.width - 8; x: 4
        y: (parent.height - height) / 2 - 2
        spacing: 4
        Text {
            width: parent.width; text: "PARAR\nTODO"
            color: content.ink
            font.family: "Inter"; font.pixelSize: 15
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.1
        }
        Text {
            width: parent.width; text: content.control.detail || ""
            color: "#ffffff"
            font.family: "Inter"; font.pixelSize: 11
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
