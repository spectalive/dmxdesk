import QtQuick

Item {
    id: content
    required property var control
    required property string ink

    DeskIcon {
        x: 12; y: (parent.height - height) / 2
        width: 24; height: 24; glyph: "cloud"
    }
    Text {
        x: 29; y: 0; width: 22; height: 28
        text: "↻"; color: "#3ee0aa"
        font.pixelSize: 21; font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        x: 54; y: 0; width: parent.width - 60; height: parent.height
        text: content.control.label.replace(/^cada\s+/i, "")
        color: content.ink
        font.family: "Inter"; font.pixelSize: 12; font.bold: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
