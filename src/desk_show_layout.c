#include "desk_show_layout.h"

#include "desk_layout.h"
#include "desk_show_accent_tile.h"
#include "desk_show_is_rig.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_section.h"
#include "desk_show_slot.h"

// The panels span the content; their tiles sit inside the 12 px padding.
#define PANEL_X DESK_CONTENT_X
#define PANEL_W DESK_CONTENT_W
#define INNER_X (PANEL_X + DESK_PANEL_PAD)
#define INNER_W (PANEL_W - 2 * DESK_PANEL_PAD)

int desk_show_compose(const struct show_map *map, int page, int haze_off, int tempo,
                      struct desk_layout *out) {
    int live = desk_show_live_page(map);
    if (live < 0)
        return -1;
    const struct map_section *states = desk_show_section(map, live, "state");
    const struct map_section *haze = desk_show_section(map, live, "haze");
    const struct map_section *accents = desk_show_section(map, live, "accents");

    // SALA AHORA: the states right-aligned in the panel, the room block in
    // what is left at their left; a state that would eat the block is not laid.
    if (states) {
        if (desk_show_panel(out, page, PANEL_X, DESK_ROOM_Y, PANEL_W, DESK_ROOM_H, "SALA AHORA",
                            (int)(states - map->page[live].section), DESK_PANEL_ROOM) != 0)
            return -1;
        int fit = (INNER_W - DESK_ROOM_BLOCK_W) / (DESK_STATE_W + DESK_GAP);
        int n = states->count < fit ? states->count : fit;
        int x = INNER_X + INNER_W - n * DESK_STATE_W - (n - 1) * DESK_GAP;
        for (int k = 0; k < n; k++)
            if (desk_show_place(out, states->first + k, page, x + k * (DESK_STATE_W + DESK_GAP),
                                DESK_ROOM_Y + DESK_PANEL_PAD, DESK_STATE_W, DESK_STATE_H,
                                TILE_STATE) != 0)
                return -1;
    }
    // GOLPES: every accent in map order, one row of equal tiles.
    if (accents && accents->count > 0) {
        if (desk_show_panel(out, page, PANEL_X, DESK_HITS_Y, PANEL_W, DESK_HITS_H,
                            "GOLPES \xc2\xb7 MIENTRAS PULSES",
                            (int)(accents - map->page[live].section), DESK_PANEL_PLAIN) != 0)
            return -1;
        for (int k = 0; k < accents->count; k++) {
            const struct map_control *c = &map->control[accents->first + k];
            int w, x = desk_show_slot(INNER_X, INNER_W, accents->count, DESK_GAP, k, &w);
            if (desk_show_place(out, accents->first + k, page, x, DESK_HITS_Y + DESK_PANEL_TOP, w,
                                DESK_HOLD_TILE_H, desk_show_accent_tile(c)) != 0)
                return -1;
        }
    }
    // HUMO AMBIENTE: OFF first, then the rhythms, one row of equal segments.
    // Choosing a rhythm fires at once; OFF stops the cycle.
    if (haze) {
        if (desk_show_panel(out, page, PANEL_X, DESK_HAZE_Y, PANEL_W, DESK_HAZE_H, haze->title,
                            (int)(haze - map->page[live].section), DESK_PANEL_PLAIN) != 0)
            return -1;
        int n = haze->count + (haze_off >= 0);
        for (int k = 0; k < n; k++) {
            int control = haze_off >= 0 ? (k == 0 ? haze_off : haze->first + k - 1) : haze->first + k;
            int w, x = desk_show_slot(INNER_X, INNER_W, n, DESK_GAP, k, &w);
            if (desk_show_place(out, control, page, x, DESK_HAZE_Y + DESK_PANEL_TOP, w,
                                DESK_SEGMENT_H, TILE_SEGMENT) != 0)
                return -1;
        }
    }
    // The bottom row: the rig's colours, ten slots, and the tempo panel.
    int rig_w = tempo >= 0 ? PANEL_W - DESK_TEMPO_W - DESK_GAP : PANEL_W;
    if (desk_show_panel(out, page, PANEL_X, DESK_RIG_Y, rig_w, DESK_RIG_H, "COLOR DEL RIG", -1,
                        DESK_PANEL_NAMES_RUNNING) != 0)
        return -1;
    int laid = 0;
    for (int i = 0; i < map->count && laid < DESK_RIG_SLOTS; i++) {
        if (!desk_show_is_rig(&map->control[i]))
            continue;
        int w, x = desk_show_slot(INNER_X, rig_w - 2 * DESK_PANEL_PAD, DESK_RIG_SLOTS, DESK_GAP,
                                  laid++, &w);
        if (desk_show_place(out, i, page, x, DESK_RIG_Y + DESK_PANEL_TOP + 4, w, DESK_MINI_H,
                            TILE_MINI) != 0)
            return -1;
    }
    if (tempo >= 0 &&
        desk_show_place(out, tempo, page, PANEL_X + PANEL_W - DESK_TEMPO_W, DESK_RIG_Y,
                        DESK_TEMPO_W, DESK_RIG_H, TILE_TEMPO) != 0)
        return -1;
    return 0;
}
