import QtQuick

Rectangle {
    id: curtain
    property int page: 0
    property int bank: 0
    color: "#0c0e12"
    opacity: 0
    visible: opacity > 0
    onPageChanged: reveal.restart()
    onBankChanged: reveal.restart()

    DeskNumberMotion {
        id: reveal
        target: curtain
        property: "opacity"
        from: 0.3; to: 0
    }
}
