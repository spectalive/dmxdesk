import QtQuick

// One baked Stage Console look (halo, recess, sheen, rim) drawn as a 9-slice
// image around its parent. The looks are rendered by qt/bake_stage_chrome.py.
BorderImage {
    property string look: ""
    property int slice: 22
    readonly property int margin: 4
    anchors.fill: parent
    anchors.margins: -margin
    source: look ? Qt.resolvedUrl("chrome/" + look + ".ico") : ""
    border.left: margin + slice; border.right: margin + slice
    border.top: margin + slice; border.bottom: margin + slice
}
