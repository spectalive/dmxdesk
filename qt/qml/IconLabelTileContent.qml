import QtQuick

Item {
    id: content
    required property string glyph
    required property string label
    required property string ink
    required property int iconSize
    required property int labelSize
    property bool active: false
    property bool pressed: false

    Column {
        objectName: "align-x-parent"
        width: parent.width - 6
        x: 3
        y: (parent.height - height) / 2
        spacing: 8
        DeskAnimatedIcon {
            width: content.iconSize; height: width
            anchors.horizontalCenter: parent.horizontalCenter
            glyph: content.glyph
            active: content.active
            pressed: content.pressed
        }
        Text {
            width: parent.width
            text: content.label
            color: content.ink
            font.family: "Inter"; font.pixelSize: content.labelSize; font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }
    }
}
