import QtQuick

Item {
    required property var settings
    x: 0; y: 0; width: 904; height: 600

    Rectangle { x: 0; y: 48; width: 904; height: 552; color: "#0c0e12" }
    Text { x: 16; y: 65; text: "AJUSTES"; color: "#f5f7fa"; font.pixelSize: 18; font.bold: true }
    Rectangle {
        x: 756; y: 56; width: 88; height: 48; radius: 12
        color: "#1b1f27"; border.color: "#39404d"
        Text { anchors.centerIn: parent; text: "Cerrar"; color: "#f5f7fa"; font.pixelSize: 13 }
    }
    Repeater {
        model: [
            {x:16, title:"Wi-Fi", current:settings.ssid || "Sin red"},
            {x:428, title:"Master QLC+", current:settings.master ? settings.master + ":" + settings.port : "Sin master"}
        ]
        delegate: Rectangle {
            x: modelData.x; y: 112; width: 404; height: 364; radius: 20
            color: "#1b1f27"; border.color: "#39404d"
            Text { x: 16; y: 13; text: modelData.title; color: "#adb6c4"; font.pixelSize: 14 }
            Text { x: 16; y: 53; width: 372; text: modelData.current; color: "#f5f7fa"; font.pixelSize: 13; elide: Text.ElideRight }
        }
    }
    Repeater {
        model: (settings.networks || []).slice((settings.scanPage || 0) * 3,
                                                 (settings.scanPage || 0) * 3 + 3)
        delegate: Rectangle {
            x: 24; y: 204 + index * 56; width: 388; height: 48; radius: 12
            color: modelData.ssid === settings.ssid ? "#262c36" : "#0c0e12"
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; width: 335
                   text: modelData.ssid; color: "#f5f7fa"; font.pixelSize: 14; elide: Text.ElideRight }
        }
    }
    Repeater {
        model: (settings.found || []).slice((settings.foundPage || 0) * 3,
                                              (settings.foundPage || 0) * 3 + 3)
        delegate: Rectangle {
            x: 436; y: 204 + index * 56; width: 388; height: 48; radius: 12
            color: modelData.host === settings.master ? "#262c36" : "#0c0e12"
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; width: 335
                   text: modelData.host + ":" + (modelData.port || settings.port)
                   color: "#f5f7fa"; font.pixelSize: 14; elide: Text.ElideRight }
        }
    }
    Rectangle {
        x: 32; y: 412; width: 372; height: 48; radius: 12
        color: "#262c36"; border.color: "#adb6c4"
        Text { anchors.centerIn: parent; text: settings.wifiBusy ? "Buscando..." : "Buscar redes"; color: "#f5f7fa"; font.pixelSize: 13 }
    }
    Rectangle {
        x: 444; y: 412; width: 178; height: 48; radius: 12
        color: "#262c36"; border.color: "#adb6c4"
        Text { anchors.centerIn: parent; text: settings.masterBusy ? "Parar" : "Buscar QLC+"; color: "#f5f7fa"; font.pixelSize: 13 }
    }
    Rectangle {
        x: 638; y: 412; width: 178; height: 48; radius: 12
        color: "#0c0e12"; border.color: "#adb6c4"
        Text { anchors.centerIn: parent; text: "Escribir IP"; color: "#f5f7fa"; font.pixelSize: 13 }
    }
    Rectangle {
        x: 16; y: 492; width: 560; height: 92; radius: 12; color: "#1b1f27"
        Text { x: 16; y: 12; text: "BRILLO"; color: "#f5f7fa"; font.pixelSize: 12 }
        Rectangle {
            x: 16; y: 53; width: 528; height: 16; radius: 8; color: "#0c0e12"
            Rectangle {
                width: parent.width * Math.max(0, Math.min(1,
                       (settings.brightness - 8) / Math.max(1, settings.brightnessMax - 8)))
                height: parent.height; radius: 8; color: "#adb6c4"
            }
        }
    }
    Rectangle {
        x: 584; y: 492; width: 260; height: 92; radius: 12; color: "#1b1f27"
        Text { x: 20; y: 16; text: "ATENUAR CON BATERÍA"; color: "#f5f7fa"; font.pixelSize: 12 }
        Text { x: 20; y: 50; text: settings.powerAware ? "Sí" : "No"; color: "#f5f7fa"; font.pixelSize: 15 }
    }
    Rectangle {
        visible: !!settings.confirmOpen
        x: 172; y: 200; width: 500; height: 220; radius: 20
        color: "#262c36"; border.color: "#adb6c4"
        Text { x: 24; y: 28; text: "¿Unirse a " + settings.confirmSsid + "?"; color: "#f5f7fa"; font.pixelSize: 18 }
        Text { x: 24; y: 78; width: 450; wrapMode: Text.WordWrap
               text: "La tablet perderá la conexión al cambiar de red; el show sigue en el Mac."
               color: "#adb6c4"; font.pixelSize: 12 }
        Text { x: 24; y: 116; text: settings.confirmOpenNetwork ? "Red abierta: sin clave." : "La clave queda en la tablet."
               color: "#adb6c4"; font.pixelSize: 12 }
        Text { x: 26; y: 177; text: "Cancelar"; color: "#f5f7fa"; font.pixelSize: 14 }
        Text { x: 379; y: 177; text: "Unirse"; color: "#f5f7fa"; font.pixelSize: 14 }
    }
    Rectangle {
        visible: !!settings.keyboardOpen
        x: 0; y: 48; width: 844; height: 552; color: "#0c0e12"
        Text { x: 16; y: 16; text: settings.keyboardTitle || ""; color: "#adb6c4"; font.pixelSize: 14 }
        Rectangle {
            x: 16; y: 56; width: 812; height: 48; radius: 12; color: "#1b1f27"
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; color: "#f5f7fa"; font.pixelSize: 16
                   text: settings.keyboardMasked ? "•".repeat(settings.keyboardLength || 0) : settings.keyboardText || "" }
        }
        Repeater {
            model: settings.keyboardKeys || []
            delegate: Rectangle {
                x: modelData.x; y: modelData.y - 48; width: modelData.w; height: modelData.h
                radius: 12; color: "#1b1f27"; border.color: "#39404d"
                opacity: modelData.enabled ? 1 : 0.5
                Text { anchors.centerIn: parent; text: modelData.label; color: "#f5f7fa"; font.pixelSize: 15 }
            }
        }
    }
}
