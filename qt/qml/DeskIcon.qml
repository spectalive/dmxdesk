import QtQuick

Image {
    property string glyph: ""
    readonly property var names: ({
        "▶": "play", "⏹": "stop", "🎤": "microphone", "🌙": "moon",
        "🎉": "party", "🔥": "flame", "💡": "bulb", "⚡": "bolt",
        "🎨": "palette", "☁": "cloud", "⇧": "arrow_up", "✳": "asterisk",
        "↔": "arrow_both", "◎": "target", "❋": "gobo", "✧": "sparkle",
        "🌈": "rainbow", "✓": "check", "◐": "half_left", "◑": "half_right",
        "◒": "half_bottom", "◓": "half_top", "▦": "grid"
    })
    property bool glow: true
    readonly property string iconName: glyph ? names[glyph.replace("️", "")] || glyph : ""
    source: iconName ? Qt.resolvedUrl("icons/" + iconName + ".ico") : ""
    fillMode: Image.PreserveAspectFit
    smooth: true
    sourceSize.width: glyph.startsWith("traj_") ? 80 : 48
    sourceSize.height: glyph.startsWith("traj_") ? 80 : 48

    // Soft halo the icon glows with, drawn beneath it.
    Image {
        z: -1
        visible: parent.glow && !!parent.iconName
        anchors.centerIn: parent
        width: parent.width * 1.6; height: parent.height * 1.6
        source: visible ? Qt.resolvedUrl("icons/glow/" + parent.iconName + ".ico") : ""
        smooth: true
    }
}
