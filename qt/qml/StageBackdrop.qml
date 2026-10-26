import QtQuick

// Window floor: near-black with a faint violet wash towards the top.
Rectangle {
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#0d0e1a" }
        GradientStop { position: 0.35; color: "#08090f" }
        GradientStop { position: 1.0; color: "#06070b" }
    }
}
