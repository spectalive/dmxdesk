import QtQuick

Item {
    id: content
    required property var control
    required property string ink

    DeskIcon {
        x: 12; y: (parent.height - height) / 2
        width: 24; height: 24; glyph: "cloud"
    }
    Rectangle {
        x: 29; y: 6; width: 18; height: 18; radius: 9
        color: "#14231f"; border.color: "#3fa88a"
        Text {
            anchors.centerIn: parent
            text: "↻"; color: "#3ee0aa"
            font.family: "Inter"; font.pixelSize: 14; font.bold: true
        }
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
