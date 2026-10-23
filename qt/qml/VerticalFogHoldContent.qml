import QtQuick

Item {
    id: content
    required property string ink
    required property string accent
    property bool active: false
    property bool pressed: false
    property string prompt: "MANTENER"

    Row {
        objectName: "align-x-parent"
        x: (parent.width - implicitWidth) / 2; y: 5
        spacing: 8
        DeskAnimatedIcon {
            objectName: "align-mirror:vertical-fog"
            y: 2
            width: 26; height: 26
            glyph: "vertical_fog"
            active: content.active; pressed: content.pressed
        }
        Text {
            objectName: "align-x-parent"
            width: 124; height: 26
            text: "HUMO VERTICAL"
            color: content.ink
            font.family: "Inter"; font.pixelSize: 13; font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            leftPadding: 8
        }
        DeskAnimatedIcon {
            objectName: "align-mirror:vertical-fog"
            y: 2
            width: 26; height: 26
            glyph: "vertical_fog"
            active: content.active; pressed: content.pressed
        }
    }
    Rectangle {
        objectName: "align-x-parent"
        x: (parent.width - width) / 2; y: 32
        width: 82; height: 18; radius: 9
        color: content.accent
        Text {
            anchors.centerIn: parent
            text: content.prompt
            color: "#1b1f27"
            font.family: "Inter"; font.pixelSize: 10; font.bold: true
        }
    }
}
