import QtQuick

// Window floor: blue-black slate with a faint violet wash towards the top.
Rectangle {
    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#0e121c" }
        GradientStop { position: 0.4; color: "#0b0f17" }
        GradientStop { position: 1.0; color: "#080a10" }
    }
}
