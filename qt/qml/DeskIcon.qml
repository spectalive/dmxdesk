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
    source: glyph ? Qt.resolvedUrl("icons/" +
                                   (names[glyph.replace("️", "")] || glyph) + ".png") : ""
    fillMode: Image.PreserveAspectFit
    smooth: true
    sourceSize.width: glyph.startsWith("traj_") ? 80 : 48
    sourceSize.height: glyph.startsWith("traj_") ? 80 : 48
}
