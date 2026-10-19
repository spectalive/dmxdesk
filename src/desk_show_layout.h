// DIRECTO: the page for the night, a fixed composition rather than a flow.
// At the left SALA AHORA, a hero panel that says what runs, large, and the
// room's states in one row under it. At the right EFECTOS · MIENTRAS PULSES,
// every LIVE accent as a pad, three to a row, a lone last one a wide pad.
// Across the bottom HUMO AMBIENTE (OFF beside the rhythms), COLOR DEL RIG,
// the rig's single colours as swatches without words, and the TEMPO SHOW
// panel. Every control comes from the map's LIVE page and COLOR's
// single-colour picks; the OFF segment and the tempo panel are the model's
// own pseudo controls, given by index.
#ifndef DESK_SHOW_LAYOUT_H
#define DESK_SHOW_LAYOUT_H

#include "desk_layout_resolve.h"
#include "desk_show_live_page.h"
#include "showmap.h"

// At most this many rig colours, in as many equal slots.
#define DESK_RIG_SLOTS 10

// Composes page `page` of `out` from the map's LIVE page (its states, haze
// rhythms and accents) and COLOR's rig colours. `haze_off` and `tempo` are
// model indices of the pseudo controls, -1 when absent. Returns 0, or -1
// when the layout has no room.
int desk_show_compose(const struct show_map *map, int page, int haze_off, int tempo,
                      struct desk_layout *out);

#endif
