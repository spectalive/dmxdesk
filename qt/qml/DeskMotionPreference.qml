import QtQuick

QtObject {
    // Older tablet runtimes do not expose the system motion preference.
    property var preference: Qt.styleHints.accessibility?.motionPreference
    readonly property bool reduced: preference !== undefined &&
                                    Qt.ReducedMotion !== undefined && preference === Qt.ReducedMotion
}
