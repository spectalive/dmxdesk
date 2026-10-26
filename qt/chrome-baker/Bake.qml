import QtQuick
import QtQuick.Window

// Renders every Stage Console look once so the desk can draw it as one
// 9-slice image. Run through qt/bake_stage_chrome.py, which saves the passes.
Window {
    id: root
    width: 200; height: 200
    visible: true
    color: "transparent"
    property string outDir: Qt.application.arguments[Qt.application.arguments.length - 1]
    property int index: 0
    readonly property int margin: 16

    // look: name, r: radius, pass: "halo" | "face" | "panel", plus component settings.
    readonly property var looks: [
        {name: "tile-idle", r: 12, depth: 1, rim: "#c8d4ff", rimStrength: 1},
        {name: "tile-off", r: 12, depth: 1, rim: "#c8d4ff", rimStrength: 0.4},
        {name: "tile-selected", r: 12, depth: 1, glow: "#8fa0ff", glowStrength: 1, rim: "#c8d4ff", rimStrength: 1},
        {name: "tile-hold", r: 12, depth: 1, glow: "#8a5cff", glowStrength: 0.8, inner: "#8a5cff", rim: "#dccfff", rimStrength: 1},
        {name: "tile-hold-lit", r: 12, depth: 1, glow: "#8a5cff", glowStrength: 1, inner: "#8a5cff", rim: "#dccfff", rimStrength: 1},
        {name: "tile-fog", r: 12, depth: 1, glow: "#2ff0c0", glowStrength: 0.8, inner: "#2ff0c0", rim: "#c4fff0", rimStrength: 1},
        {name: "tile-fog-lit", r: 12, depth: 1, glow: "#2ff0c0", glowStrength: 1, inner: "#2ff0c0", rim: "#c4fff0", rimStrength: 1},
        {name: "tile-panic", r: 12, depth: 1, glow: "#ff2a33", glowStrength: 1, sheen: 1.2, inner: "#ff8080", rim: "#ffc9c9", rimStrength: 1},
        {name: "tile-master", r: 12, depth: 1, glow: "#8a5cff", glowStrength: 0.85, rim: "#c8d4ff", rimStrength: 1},
        {name: "tile-tap", r: 12, depth: 1, glow: "#7c6cf0", glowStrength: 0.45, rim: "#c8d4ff", rimStrength: 1},
        {name: "swatch", r: 8, depth: 1.4, sheen: 0.5, rim: "#c8d4ff", rimStrength: 0.55},
        {name: "swatch-selected", r: 8, depth: 1.4, glow: "#8fa0ff", glowStrength: 1, sheen: 0.5, rim: "#c8d4ff", rimStrength: 0.55},
        {name: "tile-sheen", r: 12, depth: 0, rimStrength: 0.001},
        {name: "knob", r: 8, depth: 0, glow: "#8fe8ff", glowStrength: 0.9, sheen: 0.001, rimStrength: 0.001},
        {name: "key", r: 10, depth: 1, rim: "#c8d4ff", rimStrength: 1},
        {name: "key-dim", r: 10, depth: 1, rim: "#c8d4ff", rimStrength: 0.6},
        {name: "key-selected", r: 10, depth: 1, glow: "#a08cff", glowStrength: 0.9, rim: "#c8d4ff", rimStrength: 1},
        {name: "key-tempo", r: 10, depth: 1, glow: "#7c6cf0", glowStrength: 0.45, rim: "#c8d4ff", rimStrength: 1},
        {name: "panel", r: 16, panel: true, panelRim: "#34416a", halo: "#4f6cff"},
        {name: "panel-effects", r: 16, panel: true, panelRim: "#4b3d9c", halo: "#7c5cff"},
        {name: "panel-haze", r: 16, panel: true, panelRim: "#2c4a6a", halo: "#4f6cff"},
        {name: "panel-lit", r: 16, panel: true, lit: true, panelRim: "#34416a", halo: "#4f6cff"}
    ]
    readonly property var passes: {
        let out = []
        for (const look of looks) {
            if (look.panel) out.push({look: look, pass: "panel"})
            else { out.push({look: look, pass: "halo"}); out.push({look: look, pass: "face"}) }
        }
        return out
    }
    readonly property var current: passes[index]

    Item {
        id: canvas
        readonly property var look: root.current.look
        readonly property int inner: look.panel ? 120 : 80
        width: inner + 2 * root.margin; height: width

        Item {
            id: halo
            visible: root.current.pass === "halo"
            x: root.margin; y: root.margin; width: canvas.inner; height: canvas.inner
            StageWell { cornerRadius: canvas.look.r; depth: canvas.look.depth ?? 1 }
            StageGlow {
                visible: !!canvas.look.glow
                tint: canvas.look.glow || "transparent"
                cornerRadius: canvas.look.r
                strength: canvas.look.glowStrength || 0
            }
        }
        Item {
            id: face
            visible: root.current.pass === "face"
            x: root.margin; y: root.margin; width: canvas.inner; height: canvas.inner
            StageSheen { cornerRadius: canvas.look.r; strength: canvas.look.sheen || 1 }
            StageInnerGlow { visible: !!canvas.look.inner; tint: canvas.look.inner || "transparent"; cornerRadius: canvas.look.r }
            StageRim { cornerRadius: canvas.look.r; light: canvas.look.rim || "#c8d4ff"; strength: canvas.look.rimStrength || 1 }
        }
        PanelLook {
            visible: root.current.pass === "panel"
            x: root.margin; y: root.margin; width: canvas.inner; height: canvas.inner
            rim: canvas.look.panelRim || "#34416a"
            halo: canvas.look.halo || "#4f6cff"
            lit: !!canvas.look.lit
        }
    }

    function next() {
        canvas.grabToImage(result => {
            result.saveToFile(outDir + "/" + current.look.name + "." + current.pass + ".png")
            if (index + 1 < passes.length) { index++; Qt.callLater(next) }
            else Qt.quit()
        })
    }
    Component.onCompleted: Qt.callLater(next)
}
