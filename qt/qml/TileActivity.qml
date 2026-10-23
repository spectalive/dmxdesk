import QtQuick

Item {
    id: activity
    required property bool pressed
    required property bool available
    required property color accent
    property real progress: -1
    property real cornerRadius: 12

    Rectangle {
        anchors.fill: parent
        radius: activity.cornerRadius
        color: activity.accent
        opacity: activity.pressed && activity.available ? 0.16 : 0
        Behavior on opacity { DeskNumberMotion { milliseconds: activity.pressed ? 0 : 180 } }
    }
    Rectangle {
        anchors.fill: parent; anchors.margins: 3
        radius: Math.max(0, activity.cornerRadius - 3)
        color: "transparent"
        border.color: activity.accent
        opacity: activity.pressed && activity.available ? 0.85 : 0
        Behavior on opacity { DeskNumberMotion { milliseconds: activity.pressed ? 0 : 180 } }
    }
    Rectangle {
        visible: activity.available && activity.progress >= 0
        x: 8; y: parent.height - 8
        width: parent.width - 16; height: 3; radius: 1.5
        color: "#39404d"
        Rectangle {
            objectName: "hold-remaining"
            width: parent.width * (1 - Math.max(0, Math.min(1000, activity.progress)) / 1000)
            height: parent.height; radius: parent.radius
            color: activity.accent
            Behavior on width {
                DeskNumberMotion { milliseconds: activity.progress <= 0 || activity.progress >= 1000 ? 0 : 80 }
            }
        }
    }
}
