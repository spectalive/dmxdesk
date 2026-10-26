import QtQuick

// Black ceramic group panel with a thin coloured rim light, from a baked look.
Item {
    property string look: "panel"
    StageChrome { look: parent.look; slice: 40 }
}
