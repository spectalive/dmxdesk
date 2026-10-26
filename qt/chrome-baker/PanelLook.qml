import QtQuick

// Black ceramic group panel with a thin coloured rim light (bake source).
Rectangle {
    id: panel
    property color rim: "#34416a"
    property color halo: "#4f6cff"
    property bool lit: false
    radius: 16
    border.width: 1
    border.color: lit ? "#7c6cf0" : rim
    gradient: Gradient {
        GradientStop { position: 0.0; color: panel.lit ? "#232457" : "#151b27" }
        GradientStop { position: 1.0; color: panel.lit ? "#15163a" : "#10151f" }
    }
    StageWell { cornerRadius: panel.radius; depth: 0.9 }
    StageGlow { cornerRadius: panel.radius; tint: panel.lit ? "#7c6cf0" : panel.halo; strength: 0.25 }
    StageRim { cornerRadius: panel.radius; light: "#b8c4dc"; strength: 0.55; side: 0.6; base: "#4a6aa8"; baseStrength: 0.6 }
}
