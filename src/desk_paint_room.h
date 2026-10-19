// SALA AHORA's block: under the panel's heading, at the left of its state
// tiles, the name of the state the master says runs, as large as it fits,
// with its detail muted under it. "?" while the master has not said, or
// while there is no master to say; a dash when it says nothing runs.
#ifndef DESK_PAINT_ROOM_H
#define DESK_PAINT_ROOM_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_room(struct canvas *c, const struct desk_model *m, const struct desk_heading *h,
                     const struct desk_fonts *fonts);

#endif
