import QtQuick

Item {
    id: content
    required property var control
    property int bottomInset: 0
    readonly property bool wide: width >= 2 * height
    readonly property string mark: control.icon || "bolt"
    readonly property string ink: control.enabled ? "#f5f7fa" : "#adb6c4"
    readonly property string accent: control.tile === 9 ? "#3ee0aa" : "#a991e8"

    DeskIcon {
        visible: content.wide
        x: 16; y: (parent.height - height) / 2
        width: 25; height: 25; glyph: content.mark
    }

    Column {
        id: stack
        x: content.wide ? 57 : 4
        width: content.width - x - (content.wide ? 16 : 4)
        spacing: 4
        y: (content.height - content.bottomInset - height) / 2

        DeskIcon {
            visible: !content.wide
            width: 25; height: visible ? 25 : 0
            anchors.horizontalCenter: parent.horizontalCenter
            glyph: content.mark
        }
        Text {
            width: parent.width
            text: content.control.label || ""
            color: content.ink
            font.family: "Inter"; font.pixelSize: 13; font.bold: true
            horizontalAlignment: content.wide ? Text.AlignLeft : Text.AlignHCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }
        Rectangle {
            width: Math.min(parent.width, 82); height: 18; radius: 9
            color: content.accent
            anchors.horizontalCenter: content.wide ? undefined : parent.horizontalCenter
            Text {
                anchors.centerIn: parent
                text: "MANTENER"
                color: "#1b1f27"; font.family: "Inter"; font.pixelSize: 10; font.bold: true
            }
        }
    }
}
