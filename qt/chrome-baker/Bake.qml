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
    readonly property int margin: 4

    // look: name, r: radius, pass: "halo" | "face" | "panel", plus component settings.
    readonly property var looks: [
        {name: "tile-idle", r: 12, depth: 1, glow: "#5f7cff", glowStrength: 0.25, rim: "#c4d0ea", rimStrength: 0.95, side: 0.75, base: "#4a7ad0", baseStrength: 1},
        {name: "tile-quiet", r: 12, depth: 1, rim: "#6c7a9c", rimStrength: 1, side: 0.6, base: "#5a74b8", baseStrength: 0.8},
        {name: "tile-off", r: 12, depth: 1, rim: "#c8d4ff", rimStrength: 0.4, base: "#6f8cff", baseStrength: 0.3},
        {name: "tile-selected", r: 12, depth: 1, glow: "#8fa0ff", glowStrength: 0.45, rim: "#dfe6ff", rimStrength: 1, side: 0.8, base: "#6f8cff", baseStrength: 1},
        {name: "tile-hold", r: 12, depth: 1, glow: "#6a30ff", glowStrength: 0.6, rim: "#f2e6ff", rimStrength: 1, side: 0.85, base: "#7a50f0", baseStrength: 0.8, innerShade: 0.45},
        {name: "tile-hold-lit", r: 12, depth: 1, glow: "#7a40ff", glowStrength: 1, rim: "#ffffff", rimStrength: 1, side: 0.95, base: "#9a70ff", baseStrength: 1, innerShade: 0.3},
        {name: "tile-fog", r: 12, depth: 1, glow: "#10d8a0", glowStrength: 0.6, rim: "#7cf8e0", rimStrength: 1, side: 0.75, base: "#10b080", baseStrength: 0.9, innerShade: 0.45},
        {name: "tile-fog-lit", r: 12, depth: 1, glow: "#20f0c0", glowStrength: 1, rim: "#b0fff0", rimStrength: 1, side: 0.85, base: "#30e0b0", baseStrength: 1, innerShade: 0.3},
        {name: "tile-panic", r: 12, depth: 1, frame: 0.85, glow: "#ff2a33", glowStrength: 0.5, rim: "#ff8a92", rimStrength: 1, side: 0.8, base: "#ff3a44", baseStrength: 1},
        {name: "tile-master", r: 12, depth: 1, glow: "#8a5cff", glowStrength: 0.85, rim: "#c8d4ff", rimStrength: 1, base: "#a07cff", baseStrength: 1},
        {name: "tile-tap", r: 12, depth: 1, glow: "#7c6cf0", glowStrength: 0.45, rim: "#c8d4ff", rimStrength: 1, base: "#8f7cff", baseStrength: 1},
        {name: "swatch", r: 8, depth: 1, frame: 0.3, inset: 1.5, rim: "#ffffff", rimStrength: 0.9, side: 0.3, base: "#ffffff", baseStrength: 0.3},
        {name: "swatch-selected", r: 8, depth: 1, frame: 0.3, inset: 1.5, glow: "#8fa0ff", glowStrength: 1, rim: "#ffffff", rimStrength: 0.9, side: 0.6, base: "#ffffff", baseStrength: 0.4},
        {name: "tile-sheen", r: 12, depth: 0, rimStrength: 0.001},
        {name: "knob", r: 8, depth: 0, glow: "#8fe8ff", glowStrength: 0.9, rimStrength: 0.001},
        {name: "key", r: 10, depth: 1, rim: "#eef2ff", rimStrength: 1, base: "#8fa8ff", baseStrength: 0.9},
        // The live tiles, tabs and the plain panel are cut from the concept's
        // exported sheets by qt/slice_stage_elements.py instead.
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
            StageWell { cornerRadius: canvas.look.r; depth: canvas.look.depth ?? 1; frame: canvas.look.frame || 0 }
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
            Rectangle {
                id: glass
                visible: !!canvas.look.fill
                anchors.fill: parent
                radius: canvas.look.r
                readonly property color base: canvas.look.fill || "transparent"
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.tint(glass.base, canvas.look.top || "transparent") }
                    GradientStop { position: 0.55; color: glass.base }
                    GradientStop { position: 1.0; color: Qt.tint(glass.base, canvas.look.shade || "transparent") }
                }
            }
            StageSheen { visible: canvas.look.name === "tile-sheen"; cornerRadius: canvas.look.r }
            Rectangle {
                visible: !!canvas.look.innerShade
                x: 1; y: 1; width: parent.width - 2; height: parent.height - 2
                radius: canvas.look.r - 1
                color: "transparent"
                border.width: 1
                border.color: Qt.rgba(0, 0, 0, canvas.look.innerShade || 0)
            }
            StageRim {
                cornerRadius: canvas.look.r; light: canvas.look.rim || "#c8d4ff"; strength: canvas.look.rimStrength || 1
                side: canvas.look.side || 0
                inset: canvas.look.inset || 0
                base: canvas.look.base || "transparent"; baseStrength: canvas.look.baseStrength || 0
            }
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
