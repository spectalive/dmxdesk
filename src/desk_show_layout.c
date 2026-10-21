#include "desk_show_layout.h"

#include "desk_layout.h"
#include "desk_show_haze.h"
#include "desk_show_holds.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_rig.h"
#include "desk_show_section.h"
#include "desk_show_slot.h"

// No state is laid narrower than a finger.
#define STATE_MIN_W 48

int desk_show_compose(const struct show_map *map, int page, int haze_off, int tempo,
                      struct desk_layout *out) {
    int live = desk_show_live_page(map);
    if (live < 0)
        return -1;
    const struct map_section *states = desk_show_section(map, live, "state");
    const struct map_section *haze = desk_show_section(map, live, "haze");
    const struct map_section *accents = desk_show_section(map, live, "accents");

    // SALA AHORA: the hero, and the states in one row of equal tiles under
    // it; a state that would leave the others narrower than a finger is not laid.
    if (states) {
        if (desk_show_panel(out, page, DESK_HERO_X, DESK_HERO_Y, DESK_HERO_W, DESK_HERO_H, "SALA AHORA",
                            (int)(states - map->page[live].section), DESK_PANEL_ROOM) != 0)
            return -1;
        int fit = (DESK_HERO_W + DESK_STATE_GAP) / (STATE_MIN_W + DESK_STATE_GAP);
        int n = states->count < fit ? states->count : fit;
        for (int k = 0; k < n; k++) {
            int w, x = desk_show_slot(DESK_HERO_X, DESK_HERO_W, n, DESK_STATE_GAP, k, &w);
            if (desk_show_place(out, states->first + k, page, x, DESK_STATE_Y, w, DESK_STATE_H,
                                TILE_STATE) != 0)
                return -1;
        }
    }
    if (accents && accents->count > 0 &&
        desk_show_holds(map, accents, (int)(accents - map->page[live].section), page, out) != 0)
        return -1;
    // The rig's colours sit below the mode buttons; haze sits below effects.
    if (haze) {
        if (desk_show_haze(haze, (int)(haze - map->page[live].section), haze_off, page,
                           DESK_HITS_X, out) != 0)
            return -1;
    }
    if (desk_show_rig(map, page, DESK_CONTENT_X, DESK_HERO_W, out) != 0)
        return -1;
    if (tempo >= 0 &&
        desk_show_place(out, tempo, page, DESK_TEMPO_X, DESK_TEMPO_Y, DESK_TEMPO_W, DESK_TEMPO_H,
                        TILE_TEMPO) != 0)
        return -1;
    return 0;
}
