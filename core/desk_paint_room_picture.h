// The hero's picture of the state running, drawn from what the map knows of
// it rather than from sound the tablet cannot hear: its mark at 72 px, and
// under it its own colours (the map's "swatches") as a row of rounded bars.
// A state without a hero mark or without colours draws what it has.
#ifndef DESK_PAINT_ROOM_PICTURE_H
#define DESK_PAINT_ROOM_PICTURE_H

#include "canvas.h"
#include "desk_model.h"

// Paints `on`'s picture centred in the box at (x, y), `w` by `h`.
void desk_paint_room_picture(struct canvas *c, const struct desk_control *on, int x, int y, int w, int h);

#endif
