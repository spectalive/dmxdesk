// HUMO AMBIENTE in DIRECTO's bottom row: OFF a full-height segment at the
// panel's left, the rhythms beside it two rows deep, in map order. Choosing a
// rhythm fires at once; OFF stops the cycle.
#ifndef DESK_SHOW_HAZE_H
#define DESK_SHOW_HAZE_H

#include "desk_layout_resolve.h"
#include "showmap.h"

// Lays the panel at `x` on page `page`; `haze_off` is the OFF pseudo
// control's model index, -1 when absent. Returns 0, or -1 when out of room.
int desk_show_haze(const struct map_section *haze, int section, int haze_off,
                   int page, int x, struct desk_layout *out);

#endif
