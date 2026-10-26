import QtQuick

// Window floor: blue-black slate with a faint violet wash towards the top.
Rectangle {
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#161a28" }
        GradientStop { position: 0.4; color: "#12151e" }
        GradientStop { position: 1.0; color: "#0c0e14" }
    }
}
