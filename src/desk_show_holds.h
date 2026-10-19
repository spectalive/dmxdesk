// EFECTOS · MIENTRAS PULSES, DIRECTO's right column: a heading over every
// LIVE accent as a pad, three to a row, the light ones in map order and then
// the fog ones, so smoke closes the column. A lone pad in the last row is a
// wide pad 56 tall; the other rows share the rest of the column.
#ifndef DESK_SHOW_HOLDS_H
#define DESK_SHOW_HOLDS_H

#include "desk_layout_resolve.h"
#include "showmap.h"

// Lays `accents` (section index `section` of the LIVE page) on page `page`.
// Returns 0, or -1 when the layout has no room.
int desk_show_holds(const struct show_map *map, const struct map_section *accents, int section, int page,
                    struct desk_layout *out);

#endif
