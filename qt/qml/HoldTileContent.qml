import QtQuick

Item {
    id: content
    required property var control
    property int bottomInset: 0
    property bool active: false
    property bool pressed: false
    readonly property bool wide: width >= 2 * height
    readonly property bool compact: !wide && height < 90
    readonly property bool verticalFog: wide && control.tile === 9 && control.label === "HUMO VERT"
    readonly property string mark: control.icon || "bolt"
    readonly property string ink: control.enabled ? "#f5f7fa" : "#dbeafc"
    readonly property string accent: control.tile === 9 ? "#3cfde6" : "#c4b4ff"
    readonly property string prompt: control.holdProgress === -2 ? "SIN CONFIRMAR" :
                                     control.holdProgress >= 1000 ? "SUELTA" : "MANTENER"

    DeskAnimatedIcon {
        visible: content.wide && !content.verticalFog
        x: 16; y: (parent.height - height) / 2
        width: 25; height: 25; glyph: content.mark
        active: content.active; pressed: content.pressed
    }

    Item {
        visible: content.compact
        x: 6; y: 10; width: parent.width - 12; height: parent.height - 38
        DeskAnimatedIcon {
            x: 0; y: (parent.height - height) / 2
            width: 23; height: 23; glyph: content.mark
            active: content.active; pressed: content.pressed
        }
        Text {
            x: 28; width: parent.width - 28; height: parent.height
            text: content.control.label || ""
            color: content.ink
            font.family: "Inter"; font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }
    }
    Text {
        visible: content.compact
        x: 4; y: parent.height - 27; width: parent.width - 8; height: 18
        text: content.prompt; color: content.accent
        font.family: "Inter"; font.pixelSize: 11
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Column {
        id: stack
        objectName: content.wide ? "" : "align-x-parent"
        visible: !content.compact && !content.verticalFog
        x: content.wide ? 57 : 4
        width: content.width - x - (content.wide ? 16 : 4)
        spacing: 7
        y: (content.height - content.bottomInset - height) / 2

        DeskAnimatedIcon {
            visible: !content.wide
            width: 25; height: visible ? 25 : 0
            anchors.horizontalCenter: parent.horizontalCenter
            glyph: content.mark
            active: content.active; pressed: content.pressed
        }
        Text {
            width: parent.width
            text: content.control.label || ""
            color: content.ink
            font.family: "Inter"; font.pixelSize: 13
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
                text: content.prompt
                color: "#1b1f27"; font.family: "Inter"; font.pixelSize: 10; font.bold: true
            }
        }
    }
    VerticalFogHoldContent {
        visible: content.verticalFog
        anchors.fill: parent
        ink: content.ink
        accent: content.accent
        active: content.active; pressed: content.pressed
        prompt: content.prompt
    }
}
