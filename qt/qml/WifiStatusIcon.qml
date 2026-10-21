pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: wifiStatusRoot
    required property int bars

    Repeater {
        model: 4
        delegate: DeskIcon {
            required property int index
            x: 0; y: 0
            width: 24; height: 24
            glyph: "wifi_" + index
            opacity: wifiStatusRoot.bars > 0 && index <= wifiStatusRoot.bars ? 0.75 : 0.23
        }
    }
}
