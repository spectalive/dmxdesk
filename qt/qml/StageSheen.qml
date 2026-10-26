import QtQuick

// Ceramic finish over any tile face: a lit top edge fading into a shaded base.
Rectangle {
    property real cornerRadius: 12
    property real strength: 1
    anchors.fill: parent
    radius: cornerRadius
    color: "transparent"
    opacity: strength
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#1fffffff" }
        GradientStop { position: 0.45; color: "#04ffffff" }
        GradientStop { position: 1.0; color: "#38000000" }
    }
    Rectangle {
        x: parent.radius; y: 1
        width: parent.width - 2 * parent.radius; height: 1
        color: "#2effffff"
    }
}
