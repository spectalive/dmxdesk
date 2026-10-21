pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: content
    required property var control
    required property string ink
    readonly property var swatches: control.swatches || []

    Column {
        id: stack
        x: 8; width: parent.width - 16
        y: (content.height - height) / 2
        spacing: 8

        Rectangle {
            id: colorSquare
            visible: content.swatches.length > 0
            width: visible ? Math.max(24, Math.min(64, stack.width - 8,
                       content.height - caption.implicitHeight -
                       (detailText.visible ? detailText.implicitHeight + 8 : 0) - 17)) : 0
            height: width
            radius: 8; clip: true
            anchors.horizontalCenter: parent.horizontalCenter
            color: content.swatches.length === 1 ? content.swatches[0] : "transparent"
            border.width: 1
            border.color: "#626a78"
            Repeater {
                model: content.swatches.length > 1 ? content.swatches : []
                delegate: Rectangle {
                    id: stripe
                    required property int index
                    required property var modelData
                    x: stripe.index * colorSquare.width / content.swatches.length
                    y: 0
                    width: (stripe.index + 1) * colorSquare.width / content.swatches.length - x
                    height: colorSquare.height
                    color: stripe.modelData
                }
            }
        }
        DeskIcon {
            visible: content.swatches.length === 0 && !!content.control.icon
            width: visible ? 30 : 0; height: width
            anchors.horizontalCenter: parent.horizontalCenter
            glyph: content.control.icon || ""
        }
        Text {
            id: caption
            width: parent.width
            text: content.control.label || ""
            color: content.ink
            font.family: "Inter"; font.pixelSize: 12; font.bold: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }
        Text {
            id: detailText
            visible: !!content.control.detail
            width: parent.width; height: visible ? implicitHeight : 0
            text: content.control.detail || ""
            color: "#adb6c4"
            font.family: "Inter"; font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }
    }
}
