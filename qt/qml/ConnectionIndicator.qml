import QtQuick

Item {
    id: indicator
    required property bool linked
    DeskMotionPreference { id: motion }

    Rectangle {
        visible: !indicator.linked
        anchors.fill: parent
        radius: width / 2
        color: "transparent"; border.color: "#d42a2a"
        opacity: 0.4
        SequentialAnimation on opacity {
            running: !indicator.linked && indicator.visible && !motion.reduced
            loops: Animation.Infinite
            NumberAnimation { from: 0.15; to: 0.65; duration: 650; easing.type: Easing.InOutSine }
            NumberAnimation { from: 0.65; to: 0.15; duration: 650; easing.type: Easing.InOutSine }
        }
    }
    Rectangle {
        anchors.centerIn: parent
        width: 8; height: 8; radius: 4
        color: indicator.linked ? "#3cfde6" : "#d42a2a"
        Behavior on color { DeskColorMotion {} }
    }
}
