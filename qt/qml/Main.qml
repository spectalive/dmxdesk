import QtQuick
import QtQuick.Window

Window {
    id: root
    width: 1024
    height: 600
    visible: true
    color: "#0c0e12"
    title: "Vibra Lighting"

    readonly property var view: deskBridge.state
    readonly property var controls: view.controls || []
    readonly property var pages: view.pages || []
    readonly property var settings: view.settings || ({})
    readonly property int currentTab: pages.length > view.page ? pages[view.page].tab : 0
    readonly property bool linked: view.link === 3
    readonly property var activeState: controls.find(c => c.tile === 8 && c.selected)
    readonly property bool roomActive: linked && !!view.activeMode
    property var touchSlots: ({})

    function familyIconName(id) {
        const names = ["gear","lock","lock_open","wifi_0","wifi_1","wifi_2","wifi_3",
                       "battery","battery_cell","bolt","check","chevron","play","stop",
                       "microphone","moon","party","flame","bulb","palette","cloud",
                       "arrow_up","asterisk","arrow_both","target","gobo","sparkle",
                       "rainbow","half_left","half_right","half_bottom","half_top",
                       "grid","moving_head","prism","sliders"]
        return id >= 0 && id < names.length ? names[id] : ""
    }

    function sendPoints(kind, points) {
        for (let i = 0; i < points.length; i++) {
            const point = points[i]
            const key = String(point.pointId)
            let slot = touchSlots[key]
            if (kind === "D") {
                let used = Object.values(touchSlots)
                slot = -1
                for (let candidate = 0; candidate < 10; candidate++) {
                    if (used.indexOf(candidate) < 0) { slot = candidate; break }
                }
                if (slot < 0) continue
                touchSlots[key] = slot
            }
            if (slot === undefined || slot < 0) continue
            deskBridge.touch(kind, slot, Math.round(point.x), Math.round(point.y))
            if (kind === "U" || kind === "C") delete touchSlots[key]
        }
    }

    // The three tabs and status use the same panel coordinates as the C desk.
    Repeater {
        model: ["DIRECTO", "EFECTOS", "RITMO"]
        delegate: Rectangle {
            x: 16 + index * 144; y: 4; width: 136; height: 40; radius: 10
            color: root.currentTab === index ? "#343a46" : "#1b1f27"
            border.color: root.currentTab === index ? "#636a78" : "#1b1f27"
            Text {
                anchors.centerIn: parent; text: modelData
                color: root.currentTab === index ? "#f5f7fa" : "#adb6c4"
                font.family: "Inter"; font.pixelSize: 13; font.bold: true
            }
            Rectangle {
                visible: root.currentTab === index
                width: 32; height: 4; radius: 2; color: "#7c86d9"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom; anchors.bottomMargin: 4
            }
        }
    }
    Text {
        x: 458; y: 16; width: 290; height: 18
        text: root.linked ? (root.view.activeMode ? root.view.activeMode + " · Enlazado" : "Enlazado") : "Esperando conexión..."
        color: root.linked ? "#adb6c4" : "#d42a2a"
        horizontalAlignment: Text.AlignRight; elide: Text.ElideLeft
        font.family: "Inter"; font.pixelSize: 11
    }
    Rectangle {
        x: 756; y: 20; width: 8; height: 8; radius: 4
        color: root.linked ? "#3ee0aa" : "#d42a2a"
    }
    WifiStatusIcon { x: 777; y: 12; width: 24; height: 24; bars: Math.max(0, Math.min(3, root.view.wifiBars || 0)) }
    DeskIcon { x: 817; y: 12; width: 24; height: 24; glyph: "battery"; opacity: 0.75 }
    Text { x: 846; y: 17; width: 57; text: root.view.battery >= 0 ? root.view.battery + "%" : "--"; color: "#adb6c4"; font.pixelSize: 11 }
    DeskIcon { x: 924; y: 12; width: 24; height: 24; glyph: "gear"; opacity: 0.75 }
    DeskIcon { x: 972; y: 12; width: 24; height: 24; glyph: root.view.locked ? "lock" : "lock_open"; opacity: 0.75 }

    // DIRECTO: the fixed panels carry validated controls from the map.
    Rectangle {
        visible: root.currentTab === 0
        x: 16; y: 160; width: 516; height: 114; radius: 16
        color: root.roomActive ? "#2e3a7e" : "#14181e"
        border.width: root.roomActive ? 1 : 0
        border.color: "#7c86d9"
    }
    Rectangle { visible: root.currentTab === 0; x: 16; y: 282; width: 516; height: 302; radius: 16; color: "#14181e" }
    Rectangle { visible: root.currentTab === 0; x: 540; y: 160; width: 348; height: 264; radius: 16; color: "#14181e" }
    Rectangle { visible: root.currentTab === 0; x: 540; y: 432; width: 348; height: 152; radius: 16; color: "#14181e" }
    Rectangle {
        visible: root.currentTab === 0
        x: 40; y: 180; width: 8; height: 8; radius: 4
        color: root.linked ? "#3ee0aa" : "#d42a2a"
    }
    Text { visible: root.currentTab === 0; x: 56; y: 174; text: "ESTADO ACTUAL"; color: root.roomActive ? "#d6daf8" : "#adb6c4"; font.pixelSize: 11 }
    Text {
        visible: root.currentTab === 0
        x: 40; y: 206; width: 240; height: 36
        text: root.view.activeMode || "Sin escena activa"
        color: root.roomActive ? "#ffffff" : "#adb6c4"
        font.family: "Inter"; font.pixelSize: root.roomActive ? 30 : 19; font.bold: true
        verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
    }
    Text {
        visible: root.currentTab === 0
        x: 40; y: 244; width: 240; height: 20
        text: root.roomActive && root.activeState ? root.activeState.detail || "" :
              root.linked ? "Selecciona un modo" : ""
        color: "#d6daf8"; font.family: "Inter"; font.pixelSize: 12
        verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
    }
    Repeater {
        model: root.currentTab === 0 ? [
            {label: "EFECTO", value: root.view.activeEffect || "—"},
            {label: "HUMO", value: root.view.activeHaze || "—"},
            {label: "COLOR", value: root.view.activeColor || "—"}
        ] : []
        delegate: Item {
            x: 294; y: 180 + index * 25; width: 215; height: 24
            Text {
                x: 0; y: 0; width: 57; height: parent.height
                text: modelData.label; color: "#d6daf8"; font.pixelSize: 11
                font.family: "Inter"; verticalAlignment: Text.AlignVCenter
            }
            Text {
                x: 65; y: 0; width: parent.width - 65; height: parent.height
                text: modelData.value; color: "#d6daf8"; font.pixelSize: 11
                font.family: "Inter"; verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }
    Text {
        visible: root.currentTab === 0
        x: 28; y: 296; width: 320
        text: root.view.activeColor && root.view.activeColor !== "—" ?
              "COLOR DEL RIG · " + root.view.activeColor.toUpperCase() : "COLOR DEL RIG"
        color: "#adb6c4"; font.pixelSize: 11; elide: Text.ElideRight
    }
    Text {
        id: effectHeading
        visible: root.currentTab === 0
        x: 552; y: 166
        text: "EFECTOS · MIENTRAS PULSES"
        color: "#adb6c4"; font.pixelSize: 11
    }
    Rectangle {
        visible: effectHeading.visible
        x: effectHeading.x + effectHeading.implicitWidth + 12; y: 173
        width: 888 - x; height: 1
        color: "#39404d"
    }
    Text { visible: root.currentTab === 0; x: 552; y: 444; text: "HUMO AMBIENTE"; color: "#adb6c4"; font.pixelSize: 11 }

    // EFECTOS' headings and family rail come from the validated layout.
    Repeater {
        model: root.currentTab === 0 ? [] : root.view.headings || []
        delegate: Rectangle {
            x: modelData.x; y: modelData.y; width: modelData.w
            height: modelData.h > 0 ? modelData.h : 36; radius: 16
            color: "#14181e"
            Text { x: 12; y: 11; text: modelData.text; color: "#adb6c4"; font.pixelSize: 12 }
        }
    }
    Repeater {
        model: root.currentTab === 1 ? root.view.families || [] : []
        delegate: Rectangle {
            x: 16; y: 56 + index * 72; width: 124; height: 64; radius: 12
            color: modelData.page === root.view.page ? "#343a46" : "#1b1f27"
            border.color: modelData.page === root.view.page ? "#7a808c" : "#39404d"
            DeskIcon {
                x: 12; y: 20; width: 24; height: 24
                glyph: root.familyIconName(modelData.icon)
            }
            Text {
                x: 48; anchors.verticalCenter: parent.verticalCenter; width: 70
                text: root.pages.length > modelData.page ? root.pages[modelData.page].title : ""
                color: "#f5f7fa"; font.pixelSize: 11; horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
            }
        }
    }
    Repeater {
        model: root.view.pager || []
        delegate: Rectangle {
            visible: modelData.w > 0
            x: modelData.x; y: modelData.y; width: modelData.w; height: modelData.h
            radius: 10; color: modelData.bank === root.view.bank ? "#343a46" : "#1b1f27"
            border.color: modelData.bank === root.view.bank ? "#7a808c" : "#39404d"
            Text {
                x: modelData.caption ? 16 : 0; y: 0
                width: modelData.caption ? 24 : parent.width; height: parent.height
                text: modelData.bank + 1; color: "#f5f7fa"; font.pixelSize: 12
                horizontalAlignment: modelData.caption ? Text.AlignLeft : Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                visible: !!modelData.caption
                x: 50; y: 0; width: parent.width - 62; height: parent.height
                text: modelData.caption || ""; color: "#f5f7fa"; font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter; elide: Text.ElideRight
            }
        }
    }

    Repeater {
        model: root.controls
        delegate: ControlTile {
            colorOneShot: !!root.view.colorOneShot
            connected: root.linked
        }
    }
    Repeater {
        model: root.controls.filter(c => c.tile === 12)
        delegate: Item {
            x: modelData.x; y: modelData.y; width: modelData.w; height: modelData.h
            Text {
                x: 12; y: 19; width: 124; height: 50
                text: root.view.dials && root.view.dials.length && root.view.dials[0].known ? root.view.dials[0].bpm : "--"
                color: "#f5f7fa"; font.pixelSize: 40; font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Repeater {
                model: ["-", "+", "TAP"]
                delegate: Rectangle {
                    x: index === 0 ? parent.width - 204 : index === 1 ? parent.width - 156 : parent.width - 100
                    y: 24; width: index === 2 ? 88 : 44; height: 48; radius: 10
                    color: "#262c36"; border.color: "#39404d"
                    Text { anchors.centerIn: parent; text: modelData; color: "#f5f7fa"; font.pixelSize: index === 2 ? 12 : 16 }
                }
            }
        }
    }
    RhythmPage { visible: root.currentTab === 2; dials: root.view.dials || [] }

    SettingsSheet { visible: !!root.settings.open; settings: root.settings }
    Rectangle {
        visible: !!root.view.mismatch && root.linked
        x: 16; y: 48; width: 992; height: 536; radius: 16
        color: "#d9000000"
    }
    Rectangle {
        visible: !!root.view.mismatch && root.linked
        x: 248; y: 238; width: 528; height: 116; radius: 14
        color: "#343a46"
        Rectangle { x: 16; y: 17; width: 4; height: 82; color: "#d42a2a" }
        Text { x: 24; y: 25; width: 480; height: 32; text: "Otro show cargado en el Mac"; color: "#f5f7fa"; font.pixelSize: 17; horizontalAlignment: Text.AlignHCenter }
        Text { x: 24; y: 66; width: 480; height: 24; text: "Controles desactivados"; color: "#adb6c4"; font.pixelSize: 13; horizontalAlignment: Text.AlignHCenter }
    }
    Rectangle {
        visible: !!root.view.locked; x: 16; y: 544; width: 872; height: 40; radius: 10
        color: "#f5f7fa"
        Text { anchors.centerIn: parent; text: "Bloqueado · mantén el candado para desbloquear"; color: "#0c0e12"; font.pixelSize: 13 }
    }
    MultiPointTouchArea {
        anchors.fill: parent
        minimumTouchPoints: 1; maximumTouchPoints: 10
        onPressed: (points) => root.sendPoints("D", points)
        onUpdated: (points) => root.sendPoints("M", points)
        onReleased: (points) => root.sendPoints("U", points)
        onCanceled: (points) => {
            for (const key of Object.keys(root.touchSlots)) {
                deskBridge.touch("C", root.touchSlots[key], 0, 0)
                delete root.touchSlots[key]
            }
        }
    }
}
