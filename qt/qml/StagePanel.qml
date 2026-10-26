import QtQuick

// Black ceramic group panel with a thin coloured rim light, from a baked look;
// the sliced concept panels keep a wider halo.
Item {
    property string look: "panel"
    StageChrome { look: parent.look; slice: 40; margin: look === "panel" ? 8 : 4 }
}
