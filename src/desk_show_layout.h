// DIRECTO puts the mode buttons and tempo first. A compact SALA AHORA card
// sits over the rig colours on the left; effects and timed ambient haze fill
// the right. The rig panel includes a local colour-hit switch. Every control comes
// from the map's LIVE page and COLOR's
// single-colour picks; the OFF segment and the tempo panel are the model's
// own pseudo controls, given by index.
#ifndef DESK_SHOW_LAYOUT_H
#define DESK_SHOW_LAYOUT_H

#include "desk_layout_resolve.h"
#include "desk_show_live_page.h"
#include "showmap.h"

// At most this many rig colours, in as many equal slots.
#define DESK_RIG_SLOTS 17

// Composes page `page` of `out` from the map's LIVE page (its states, haze
// rhythms and accents) and COLOR's rig colours. `haze_off` and `tempo` are
// model indices of the pseudo controls, -1 when absent. Returns 0, or -1
// when the layout has no room.
int desk_show_compose(const struct show_map *map, int page, int haze_off, int tempo,
                      struct desk_layout *out);

#endif
