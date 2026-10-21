import QtQuick

Item {
    id: content
    required property string glyph
    required property string label
    required property string ink
    required property int iconSize
    required property int labelSize

    Column {
        width: parent.width - 6
        x: 3
        y: (parent.height - height) / 2
        spacing: 4
        DeskIcon {
            width: content.iconSize; height: width
            anchors.horizontalCenter: parent.horizontalCenter
            glyph: content.glyph
        }
        Text {
            width: parent.width
            text: content.label
            color: content.ink
            font.family: "Inter"; font.pixelSize: content.labelSize
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }
    }
}
