pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: wifiStatusRoot
    required property int bars

    Repeater {
        model: 4
        delegate: DeskIcon {
            required property int index
            // A wider, shallower fan balances the adjacent status marks.
            x: (wifiStatusRoot.width - width) / 2
            y: (wifiStatusRoot.height - height) / 2 + wifiStatusRoot.height / 24
            width: wifiStatusRoot.width * 26 / 24
            height: wifiStatusRoot.height * 21 / 24
            fillMode: Image.Stretch
            glyph: "wifi_" + index
            opacity: wifiStatusRoot.bars > 0 && index <= wifiStatusRoot.bars ? 0.75 : 0.23
        }
    }
}
