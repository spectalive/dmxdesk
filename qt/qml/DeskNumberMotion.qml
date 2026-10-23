import QtQuick

NumberAnimation {
    property int milliseconds: 180
    property QtObject preference: DeskMotionPreference {}
    duration: preference.reduced ? 0 : milliseconds
    easing.type: Easing.OutCubic
}
