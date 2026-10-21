import QtQuick

Item {
    id: content
    required property var control
    required property string ink

    DeskIcon {
        x: 9; y: (parent.height - height) / 2
        width: 32; height: 32; glyph: "cloud_loop"
    }
    Text {
        x: 51; y: 0; width: parent.width - 57; height: parent.height
        text: content.control.label.replace(/^cada\s+/i, "")
        color: content.ink
        font.family: "Inter"; font.pixelSize: 12; font.bold: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
