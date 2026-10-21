// COLOR DEL RIG below the mode buttons: the rig's single colours from
// COLOR, 17 slots in three rows, plus a local colour-hit switch.
// Its heading names the colour running.
#ifndef DESK_SHOW_RIG_H
#define DESK_SHOW_RIG_H

#include "desk_layout_resolve.h"
#include "showmap.h"

// Lays the panel at `x`, `w` wide, on page `page`. Returns 0, or -1 when
// the layout has no room.
int desk_show_rig(const struct show_map *map, int page, int x, int w, struct desk_layout *out);

#endif
