// A hit, the hold look: a pad in its own field (violet for light, teal for
// smoke) with its mark large, its name, a MANTENER pill and, for a burst the
// master ends by itself, how long it may last ("máx 8 s"). Held on the Mac
// it is dead and says so; capped it says to let go; an off it could not
// confirm it says in red. While the finger is down a thin bar counts the cap
// down in the place of the length; while the master says it runs it wears
// the frosted selected fill. A pad twice as wide as it is tall lays its mark
// at the left and its words beside it.
#ifndef DESK_PAINT_HOLD_H
#define DESK_PAINT_HOLD_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_hold(struct canvas *c, const struct desk_control *ctl, const struct desk_placement *p,
                     const struct desk_fonts *fonts, enum desk_link link);

#endif
