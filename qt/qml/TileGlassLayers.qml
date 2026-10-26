import QtQuick

// The tiles' decoration, drawn beneath every tile's content and grouped by
// kind - rings, live fills, baked looks - so the scene graph can draw each
// group in one batch instead of one draw per tile.
Item {
    readonly property alias rings: rings
    readonly property alias fills: fills
    readonly property alias looks: looks
    Item { id: rings }
    Item { id: fills }
    Item { id: looks }
}
