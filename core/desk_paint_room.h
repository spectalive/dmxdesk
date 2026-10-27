// DIRECTO's status panel: a link dot, the active state, and live effect,
// smoke and rig-colour summaries from the master's confirmed function state.
// While a state runs the panel is the deep indigo hero fill inside an accent
// hairline; otherwise it is a plain panel and the name is muted: "?" while
// the master has not said, or while there is no master to say; a dash when
// it says nothing runs.
#ifndef DESK_PAINT_ROOM_H
#define DESK_PAINT_ROOM_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_room(struct canvas *c, const struct desk_model *m, const struct desk_heading *h,
                     const struct desk_fonts *fonts);

#endif
