import QtQuick

Item {
    id: animatedIcon
    required property string glyph
    property bool active: false
    property bool pressed: false
    property real activation: 1

    DeskIcon {
        id: mark
        objectName: "animated-mark"
        anchors.fill: parent
        glyph: animatedIcon.glyph
        scale: animatedIcon.pressed ? 0.88 : animatedIcon.active ? 1.04 : 1
        rotation: animatedIcon.active ? -10 * (1 - animatedIcon.activation) : 0
        Behavior on scale { DeskNumberMotion { milliseconds: animatedIcon.pressed ? 65 : 180 } }
        DeskNumberMotion {
            target: animatedIcon
            property: "activation"
            from: 0; to: 1
            milliseconds: 260
            running: animatedIcon.active && animatedIcon.visible
        }
    }
}
