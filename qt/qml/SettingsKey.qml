import QtQuick

// A settings button in the Stage Console key look, the same face as the
// rhythm page's multiplier key. Geometry comes from the caller, which takes
// it from the C desk's touch targets.
Rectangle {
    property string label: ""
    radius: 12
    color: "#151a28"; border.color: "#3a4775"
    StageChrome { look: "key"; slice: 18 }
    Text { anchors.centerIn: parent; text: parent.label; color: "#f5f7fa"; font.pixelSize: 13; font.bold: true }
}
