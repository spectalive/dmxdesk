import QtQuick

// Every rectangle here is a touch target the C desk owns (desk_settings
// layout); only the faces are the Qt desk's, in the Stage Console look the
// other pages use.
Item {
    required property var settings
    x: 0; y: 0; width: 904; height: 600

    // The window floor, so the sheet covers the page below it in the same
    // material rather than a flat fill.
    Item {
        x: 0; y: 48; width: 904; height: 552; clip: true
        StageBackdrop { x: 0; y: -48; width: 1024; height: 600; anchors.fill: undefined }
    }
    Text { x: 16; y: 65; text: "AJUSTES"; color: "#f5f7fa"; font.pixelSize: 18; font.bold: true }
    SettingsKey { x: 756; y: 56; width: 88; height: 48; label: "Cerrar" }
    Repeater {
        // Counts, not arrays built in bindings: see Main.qml's Repeaters.
        model: 2
        delegate: StagePanel {
            required property int index
            x: [16, 428][index]; y: 112; width: 404; height: 364
            Text { x: 16; y: 13; text: ["WI-FI", "MASTER QLC+"][parent.index]; color: "#dbeafc"; font.pixelSize: 11 }
            Text {
                x: 16; y: 53; width: 372; color: "#f5f7fa"; font.pixelSize: 15; font.bold: true
                elide: Text.ElideRight
                text: parent.index === 0 ? settings.ssid || "Sin red" :
                      settings.master ? settings.master + ":" + settings.port : "Sin master"
            }
        }
    }
    Repeater {
        model: Math.max(0, Math.min(3, (settings.networks || []).length - (settings.scanPage || 0) * 3))
        delegate: Rectangle {
            required property int index
            readonly property var modelData: settings.networks[(settings.scanPage || 0) * 3 + index] || ({})
            readonly property bool current: modelData.ssid === settings.ssid
            x: 24; y: 204 + index * 56; width: 388; height: 48; radius: 12
            color: "#1a1e33"; border.color: current ? "#6c5ce8" : "#3a4775"
            StageChrome { look: parent.current ? "tile-selected" : "tile-idle" }
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; width: 335
                   text: modelData.ssid; color: "#f5f7fa"; font.pixelSize: 14; elide: Text.ElideRight }
        }
    }
    Repeater {
        model: Math.max(0, Math.min(3, (settings.found || []).length - (settings.foundPage || 0) * 3))
        delegate: Rectangle {
            required property int index
            readonly property var modelData: settings.found[(settings.foundPage || 0) * 3 + index] || ({})
            readonly property bool current: modelData.host === settings.master
            x: 436; y: 204 + index * 56; width: 388; height: 48; radius: 12
            color: "#1a1e33"; border.color: current ? "#6c5ce8" : "#3a4775"
            StageChrome { look: parent.current ? "tile-selected" : "tile-idle" }
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; width: 335
                   text: modelData.host + ":" + (modelData.port || settings.port)
                   color: "#f5f7fa"; font.pixelSize: 14; elide: Text.ElideRight }
        }
    }
    SettingsKey { x: 32; y: 412; width: 372; height: 48; label: settings.wifiBusy ? "Buscando..." : "Buscar redes" }
    SettingsKey { x: 444; y: 412; width: 178; height: 48; label: settings.masterBusy ? "Parar" : "Buscar QLC+" }
    SettingsKey { x: 638; y: 412; width: 178; height: 48; label: "Escribir IP" }
    StagePanel {
        x: 16; y: 492; width: 560; height: 92
        Text { x: 16; y: 12; text: "BRILLO"; color: "#dbeafc"; font.pixelSize: 11 }
        Rectangle {
            x: 16; y: 53; width: 528; height: 16; radius: 8; color: "#151a28"; border.color: "#3a4775"
            Rectangle {
                width: parent.width * Math.max(0, Math.min(1,
                       (settings.brightness - 8) / Math.max(1, settings.brightnessMax - 8)))
                height: parent.height; radius: 8; color: "#6c5ce8"
            }
        }
    }
    StagePanel {
        x: 584; y: 492; width: 260; height: 92
        Text { x: 20; y: 16; text: "ATENUAR CON BATERÍA"; color: "#dbeafc"; font.pixelSize: 11 }
        Text { x: 20; y: 50; text: settings.powerAware ? "Sí" : "No"; color: "#f5f7fa"; font.pixelSize: 15; font.bold: true }
    }
    Rectangle {
        visible: !!settings.confirmOpen
        x: 172; y: 200; width: 500; height: 220; radius: 20
        color: "#11141f"; border.color: "#6c5ce8"
        StageChrome { look: "panel-lit"; slice: 40; margin: 8 }
        Text { x: 24; y: 28; text: "¿Unirse a " + settings.confirmSsid + "?"; color: "#f5f7fa"; font.pixelSize: 18; font.bold: true }
        Text { x: 24; y: 78; width: 450; wrapMode: Text.WordWrap
               text: "La tablet perderá la conexión al cambiar de red; el show sigue en el Mac."
               color: "#dbeafc"; font.pixelSize: 12 }
        Text { x: 24; y: 116; text: settings.confirmOpenNetwork ? "Red abierta: sin clave." : "La clave queda en la tablet."
               color: "#dbeafc"; font.pixelSize: 12 }
        Text { x: 26; y: 177; text: "Cancelar"; color: "#f5f7fa"; font.pixelSize: 14; font.bold: true }
        Text { x: 379; y: 177; text: "Unirse"; color: "#f5f7fa"; font.pixelSize: 14; font.bold: true }
    }
    Item {
        visible: !!settings.keyboardOpen
        x: 0; y: 48; width: 904; height: 552; clip: true
        StageBackdrop { x: 0; y: -48; width: 1024; height: 600; anchors.fill: undefined }
        Text { x: 16; y: 16; text: settings.keyboardTitle || ""; color: "#dbeafc"; font.pixelSize: 14 }
        Rectangle {
            x: 16; y: 56; width: 812; height: 48; radius: 12; color: "#151a28"; border.color: "#3a4775"
            Text { x: 16; anchors.verticalCenter: parent.verticalCenter; color: "#f5f7fa"; font.pixelSize: 16
                   text: settings.keyboardMasked ? "•".repeat(settings.keyboardLength || 0) : settings.keyboardText || "" }
        }
        Repeater {
            model: (settings.keyboardKeys || []).length
            delegate: SettingsKey {
                required property int index
                readonly property var modelData: settings.keyboardKeys[index] || ({})
                x: modelData.x; y: modelData.y - 48; width: modelData.w; height: modelData.h
                opacity: modelData.enabled ? 1 : 0.5
                label: modelData.label === "cancel" ? "Cancelar" :
                       modelData.label === "done" ? "Aceptar" :
                       modelData.label === "del" ? "Borrar" : modelData.label
            }
        }
    }
}
