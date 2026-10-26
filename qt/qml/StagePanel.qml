import QtQuick

// Black ceramic group panel with a thin coloured rim light.
Rectangle {
    id: panel
    property color rim: "#2b3050"
    property bool lit: false
    radius: 16
    border.width: 1
    border.color: lit ? "#7c6cf0" : rim
    gradient: Gradient {
        GradientStop { position: 0.0; color: panel.lit ? "#232457" : "#151827" }
        GradientStop { position: 1.0; color: panel.lit ? "#15163a" : "#0b0d16" }
    }
    Rectangle {
        x: panel.radius; y: 1
        width: panel.width - 2 * panel.radius; height: 1
        color: "#26ffffff"
    }
}
