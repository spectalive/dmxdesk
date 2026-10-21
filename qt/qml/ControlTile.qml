import QtQuick

Rectangle {
    id: tile
    required property var modelData
    readonly property var control: modelData
    required property bool colorOneShot
    required property bool connected
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
    readonly property bool haze: control.tile === 10 || control.tile === 6
    readonly property var swatches: control.swatches || []
    readonly property string ink: control.enabled && connected ? "#f5f7fa" : "#adb6c4"

    x: control.x; y: control.y; width: control.w; height: control.h
    radius: mini ? 8 : 12
    color: mini && swatches.length ? selected ? "#7c86d9" : swatches[0] :
           panic ? connected && control.enabled ? "#d42a2a" : "#1b1f27" :
           selected ? "#343a46" : hold ? "#1f1b2b" : "#1b1f27"
    border.width: selected && !mini ? 2 : 1
    border.color: panic ? color : selected ? "#7a808c" : hold ? "#7860c8" : "#39404d"
    opacity: control.enabled && connected ? 1 : 0.6

    Rectangle {
        visible: tile.selected && !tile.pick && !tile.mini && !tile.master
        anchors.top: parent.top; anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: Math.max(18, parent.width - 36); height: 3; radius: 2
        color: "#7c86d9"
    }
    DeskIcon {
        visible: !!tile.control.icon && !tile.pick && !tile.stateTile && !tile.hold && !tile.mini && !tile.master && !tile.panic && !tile.tempo && !tile.trajectory
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
        iconSize: 40
        labelSize: tile.stateTile && label.indexOf(" ") < 0 && label.length >= 9 ? 10 :
                   tile.stateTile ? 11 : 12
    }
    PickTileContent {
        visible: tile.pick
        anchors.fill: parent
        control: tile.control
        ink: tile.ink
    }
    Text {
        visible: !tile.pick && !tile.stateTile && !tile.trajectory && !tile.hold && !tile.mini && !tile.master && !tile.panic && !tile.tempo
        x: tile.panic ? 5 : (tile.control.icon ? 46 : 8)
        width: parent.width - x - 5
        y: (parent.height - height) / 2
        text: tile.control.label === "Insert Prism" ? "Insertar prisma" : tile.control.label || ""
        color: tile.ink
        font.family: "Inter"
        font.pixelSize: 12
        font.bold: tile.panic
        horizontalAlignment: tile.panic ? Text.AlignHCenter : Text.AlignLeft
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
    Rectangle {
        visible: tile.mini && tile.colorOneShot
        x: parent.width - 30; y: 5; width: 24; height: 24; radius: 7
        color: "#0c0e12"
        DeskIcon { anchors.centerIn: parent; width: 18; height: 18; glyph: "⚡" }
    }
    Rectangle {
        visible: tile.mini && tile.selected
        x: 5; y: parent.height - 29; width: 24; height: 24; radius: 7
        color: "#0c0e12"
        DeskIcon { anchors.centerIn: parent; width: 18; height: 18; glyph: "✓" }
    }
    Text {
        visible: tile.master
        x: 0; y: 12; width: parent.width
        text: "MASTER"; color: "#adb6c4"; font.pixelSize: 11
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        visible: tile.master
        x: 0; y: 43; width: parent.width
        text: tile.connected ? Math.round(tile.control.level * 100 / 255) + "%" : "--"
        color: tile.ink; font.pixelSize: 18; font.bold: true
        horizontalAlignment: Text.AlignHCenter
    }
    Rectangle {
        visible: tile.master
        x: 16; y: 77; width: parent.width - 32; height: parent.height - 93
        radius: 8; color: "#0c0e12"
    }
    Rectangle {
        visible: tile.master && tile.control.state !== 0
        x: 12; width: parent.width - 24; height: 24; radius: 8
        y: 77 + (255 - tile.control.level) * (parent.height - 93 - 24) / 255
        color: tile.connected ? "#f5f7fa" : "#adb6c4"
    }
    Text {
        visible: tile.tempo
        x: 12; y: 11; text: tile.control.label; color: "#adb6c4"
        font.pixelSize: 10
    }
    Text {
        visible: tile.tempo
        x: 12; y: 70; width: 124
        text: "BPM BASE"; color: "#adb6c4"; font.pixelSize: 10
        horizontalAlignment: Text.AlignHCenter
    }
    Rectangle {
        visible: tile.control.pending
        x: parent.width - 8; y: 4; width: 4; height: 4; radius: 2
        color: "#7c86d9"
    }
}
