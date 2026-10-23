import QtQuick

ColorAnimation {
    property int milliseconds: 160
    property QtObject preference: DeskMotionPreference {}
    duration: preference.reduced ? 0 : milliseconds
    easing.type: Easing.OutCubic
}
