import QtQuick

Rectangle {
    id: tile
    required property var modelData
    readonly property var control: modelData
    required property bool colorOneShot
    required property bool connected
    // Where the ring, fill and baked look are drawn; see TileGlassLayers.
    required property TileGlassLayers glassLayers
    property string bpmText: "--"
    readonly property bool selected: !!control.selected && control.enabled && connected &&
                                     control.tile !== 13 && control.tile !== 14 && control.tile !== 12
    readonly property bool mini: control.tile === 11
    readonly property bool stateTile: control.tile === 8
    readonly property bool pick: control.tile === 2
    readonly property bool master: control.tile === 13
    readonly property bool panic: control.tile === 14
    readonly property bool tempo: control.tile === 12
    readonly property bool trajectory: control.tile === 4
    readonly property bool burst: control.tile === 7 && control.role === 7 && !control.icon
    readonly property var trajectoryNames: [
        "traj_circle", "traj_circle_simultaneous", "traj_circle_alternate",
        "traj_eight", "traj_eight_simultaneous", "traj_eight_alternate",
        "traj_line", "traj_line_simultaneous", "traj_line_alternate",
        "traj_diamond", "traj_diamond_simultaneous", "traj_diamond_alternate",
        "traj_square", "traj_square_simultaneous", "traj_square_alternate",
        "traj_leaf", "traj_leaf_simultaneous", "traj_leaf_alternate",
        "traj_lissajous", "traj_lissajous_simultaneous", "traj_lissajous_alternate"
    ]
    readonly property bool hold: control.tile === 7 || control.tile === 9
    readonly property bool pressed: !!control.pressed && control.enabled && connected &&
                                    !(hold && control.holdProgress >= 1000)
    readonly property bool haze: control.tile === 10 || control.tile === 6
    readonly property bool hazeOff: haze && control.label === "OFF"
    readonly property bool ambientLoop: haze && /^cada\s+\d+\s+min$/i.test(control.label || "")
    readonly property var swatches: control.swatches || []
    readonly property string ink: control.enabled && connected ? "#f5f7fa" : "#dbeafc"

    x: control.x; y: control.y; width: control.w; height: control.h
    radius: mini ? 8 : 12
    readonly property bool wideFog: control.tile === 9 && width >= 2 * height
    // Live tiles take their glass from the baked look; only swatches, whose
    // colour is live, and disabled tiles draw their own fill.
    readonly property bool live: connected && control.enabled
    readonly property bool bakedGlass: live && !mini
    color: "transparent"
    property color fill: mini && swatches.length ? selected ? "#a08cff" : swatches[0] :
           panic ? connected && control.enabled ? "#dc141d" : "#151a28" :
           selected ? "#323a50" : master ? "#1a1f32" : tempo ? "#181e2d" : wideFog ? "#062420" : control.tile === 9 ? "#062622" :
           hold ? "#18122c" : "#171c2b"
    // Glass fill: the live colour, lit at the top and shaded at the base;
    // effects stay deep and pick up their own light from below.
    readonly property color glassTop: Qt.tint(fill, mini ? "#1cffffff" : hold ? "#0cffffff" : master ? "#205a6cff" : tempo ? "#0affffff" :
                                                    ambientLoop ? "#08ffffff" : "#18ffffff")
    readonly property color glassBase: Qt.tint(fill, mini ? "#00000000" : control.tile === 9 ? "#382ff0a0" :
                                                     hold ? "#188a5cff" : panic ? "#40000000" : tempo ? "#18000000" : "#40000000")
    opacity: control.enabled && connected ? 1 : 0.6
    Behavior on fill { DeskColorMotion { milliseconds: tile.panic ? 0 : 160 } }

    // A swatch lights the recess around it in its own colour.
    Rectangle {
        parent: tile.glassLayers.rings
        visible: tile.visible && tile.mini && tile.swatches.length > 0
        x: tile.x - 3; y: tile.y - 3; width: tile.width + 6; height: tile.height + 6
        radius: tile.radius + 3
        color: tile.fill
        opacity: 0.3 * tile.opacity
    }
    Rectangle {
        parent: tile.glassLayers.fills
        visible: tile.visible && !tile.bakedGlass
        x: tile.x; y: tile.y; width: tile.width; height: tile.height
        radius: tile.radius
        opacity: tile.opacity
        gradient: Gradient {
            GradientStop { position: 0.0; color: tile.glassTop }
            GradientStop { position: 0.55; color: tile.fill }
            GradientStop { position: 1.0; color: tile.glassBase }
        }
    }
    Item {
        parent: tile.glassLayers.looks
        visible: tile.visible
        x: tile.x; y: tile.y; width: tile.width; height: tile.height
        opacity: tile.opacity
        StageChrome {
            readonly property bool lit: tile.selected || tile.pressed
            // Looks sliced from the concept's tile sheet keep a wider glow.
            margin: tile.bakedGlass ? 12 : 4
            look: tile.mini ? (tile.live && tile.selected ? "swatch-selected" : "swatch") :
                  !tile.live ? "tile-off" : tile.panic ? "fill-panic" :
                  tile.hold ? (tile.wideFog ? (lit ? "fill-fog-wide-lit" : "fill-fog-wide") :
                               tile.control.tile === 9 ? (lit ? "fill-fog-lit" : "fill-fog") :
                                                         (lit ? "fill-hold-lit" : "fill-hold")) :
                  tile.selected ? (tile.haze ? "fill-quiet-selected" : "fill-selected") : tile.master ? "fill-master" : tile.tempo ? "fill-tempo" :
                  tile.ambientLoop ? "fill-quiet" : "fill-idle"
        }
    }
    TileActivity {
        objectName: "tile-activity"
        anchors.fill: parent
        pressed: tile.pressed
        available: tile.connected && tile.control.enabled
        accent: tile.panic ? "#ffffff" : tile.control.tile === 9 ? "#3cfde6" :
                tile.mini && tile.swatches.length ? "#ffffff" : "#c4b4ff"
        cornerRadius: tile.radius
        progress: tile.hold ? tile.control.holdProgress ?? -1 : -1
    }

    Rectangle {
        objectName: "selection-marker"
        visible: opacity > 0 && !tile.pick && !tile.mini && !tile.master
        opacity: tile.selected ? 1 : 0
        anchors.top: parent.top; anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: tile.selected ? Math.max(18, parent.width - 36) : 0; height: 3; radius: 2
        color: "#8fa0ff"
        Behavior on width { DeskNumberMotion {} }
        Behavior on opacity { DeskNumberMotion {} }
    }
    DeskIcon {
        visible: !!tile.control.icon && !tile.ambientLoop && !tile.pick && !tile.stateTile && !tile.hold && !tile.mini && !tile.master && !tile.panic && !tile.tempo && !tile.trajectory
        glyph: tile.control.icon
        width: 24
        height: width
        x: 12
        y: (parent.height - height) / 2
    }
    IconLabelTileContent {
        visible: tile.stateTile || tile.trajectory
        anchors.fill: parent
        glyph: tile.trajectory && tile.control.trajectory >= 0 ?
               tile.trajectoryNames[tile.control.trajectory] : tile.control.icon || ""
        label: tile.control.label || ""
        ink: tile.ink
        iconSize: 44
        active: tile.selected
        pressed: tile.pressed
        labelSize: tile.stateTile && label.indexOf(" ") < 0 && label.length >= 9 ? 10 :
                   tile.stateTile ? 11 : 12
    }
    PickTileContent {
        visible: tile.pick
        anchors.fill: parent
        control: tile.control
        ink: tile.ink
    }
    AmbientLoopTileContent {
        visible: tile.ambientLoop
        anchors.fill: parent
        control: tile.control
        ink: tile.ink
        active: tile.selected
    }
    Text {
        visible: !tile.ambientLoop && !tile.pick && !tile.stateTile && !tile.trajectory && !tile.hold && !tile.mini && !tile.master && !tile.panic && !tile.tempo
        readonly property bool centred: tile.hazeOff || !tile.control.icon
        objectName: tile.hazeOff ? "align-x-parent" : ""
        x: centred ? 0 : tile.panic ? 5 : 46
        width: centred ? parent.width : parent.width - x - 5
        y: (parent.height - height) / 2
        text: tile.control.label === "Insert Prism" ? "Insertar prisma" : tile.control.label || ""
        color: tile.ink
        font.family: "Inter"
        font.pixelSize: 12
        font.bold: tile.panic
        horizontalAlignment: centred || tile.panic ? Text.AlignHCenter : Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        elide: Text.ElideRight
    }
    Rectangle {
        visible: tile.burst && tile.swatches.length > 0
        x: (parent.width - 32) / 2; y: parent.height - 11
        width: 32; height: 4; radius: 2; color: tile.swatches.length ? tile.swatches[0] : "transparent"
    }
    HoldTileContent {
        visible: tile.hold
        anchors.fill: parent
        control: tile.control
        bottomInset: tile.burst ? 12 : 0
        active: tile.selected
        pressed: tile.pressed
    }
    StopTileContent {
        visible: tile.panic
        anchors.fill: parent
        control: tile.control
        ink: tile.ink
    }
    Rectangle {
        visible: tile.mini && tile.selected && tile.swatches.length > 0
        x: 3; y: 3; width: parent.width - 6; height: parent.height - 6
        radius: 7; color: tile.swatches.length ? tile.swatches[0] : "transparent"
    }
    // Hits on: the bolt sits on the swatch itself, centred, with no plate;
    // dark wherever the core finds it contrasts more than white (2026-10-28).
    readonly property bool lightSwatch: !!control.markDark
    DeskIcon {
        visible: tile.mini && tile.colorOneShot && !tile.lightSwatch
        anchors.centerIn: parent
        width: 26; height: 26; glyph: "⚡"
    }
    Image {
        visible: tile.mini && tile.colorOneShot && tile.lightSwatch
        anchors.centerIn: parent
        width: 26; height: 26
        source: Qt.resolvedUrl("icons/bolt_dark.ico")
        fillMode: Image.PreserveAspectFit; smooth: true
        sourceSize.width: 48; sourceSize.height: 48
    }
    Text {
        visible: tile.master
        objectName: "align-x-parent"
        x: 0; y: 12; width: parent.width
        text: "MASTER"; color: "#dbeafc"; font.pixelSize: 11
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        visible: tile.master
        objectName: "align-x-parent"
        x: 0; y: 43; width: parent.width
        text: tile.connected ? Math.round(tile.control.level * 100 / 255) + "%" : "--"
        color: tile.ink; font.pixelSize: 18; font.bold: true
        horizontalAlignment: Text.AlignHCenter
    }
    Rectangle {
        visible: tile.master
        x: 16; y: 77; width: parent.width - 32; height: parent.height - 93
        radius: 8; color: "#07090e"
        border.color: "#2a3350"
        Rectangle { x: 6; y: 6; width: parent.width - 12; height: parent.height - 12; radius: 5; color: "#0d1018"; border.color: "#1c2234" }
    }
    Rectangle {
        visible: tile.master && tile.control.state !== 0
        x: 12; width: parent.width - 24; height: 24; radius: 8
        y: 77 + (255 - tile.control.level) * (parent.height - 93 - 24) / 255
        color: tile.connected ? "#f5f7fa" : "#dbeafc"
        StageChrome { visible: tile.connected; look: "knob"; slice: 8 }
    }
    TempoTileContent {
        visible: tile.tempo
        anchors.fill: parent
        label: tile.control.label || ""
        bpm: tile.bpmText
    }
}
