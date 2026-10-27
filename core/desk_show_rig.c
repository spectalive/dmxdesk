#include "desk_show_rig.h"

#include "desk_layout.h"
#include "desk_show_is_rig.h"
#include "desk_show_layout.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_slot.h"

#include <string.h>

#define RIG_COLUMNS 6
// The swatch slots start under the heading and button and keep a fixed
// height, leaving the panel's spare room below the grid.
#define RIG_TOP 61
#define RIG_SLOT_H 65

int desk_show_rig(const struct show_map *map, int page, int x, int w, struct desk_layout *out) {
    if (desk_show_panel(out, page, x, DESK_RIG_Y, w, DESK_RIG_H, "COLOR DEL RIG", -1,
                        DESK_PANEL_NAMES_RUNNING) != 0)
        return -1;
    if (desk_show_place(out, map->count + 4, page, x + w - 166, DESK_RIG_Y + 12,
                        154, 44, TILE_COMPACT) != 0)
        return -1;
    int x0 = x + DESK_PANEL_PAD;
    int inner_w = w - 2 * DESK_PANEL_PAD;
    int per_row = RIG_COLUMNS;
    int h = RIG_SLOT_H;
    int top = DESK_RIG_Y + RIG_TOP;
    int laid = 0;
    for (int i = 0; i < map->count && laid < DESK_RIG_SLOTS; i++) {
        if (!desk_show_is_rig(&map->control[i]))
            continue;
        int row = laid / per_row, sw;
        int sx = desk_show_slot(x0, inner_w, per_row, DESK_GAP, laid++ % per_row, &sw);
        if (desk_show_place(out, i, page, sx + 5, top + row * (h + DESK_GAP) + 5,
                            sw - 10, h - 10, TILE_MINI) != 0)
            return -1;
    }
    // White is the explicit manual look, never a step in an automatic wheel.
    for (int i = 0; i < map->count && laid < DESK_RIG_SLOTS; i++) {
        if (strcmp(map->control[i].key, "blanco-total") != 0 || map->control[i].swatches != 1)
            continue;
        int row = laid / per_row, sw;
        int sx = desk_show_slot(x0, inner_w, per_row, DESK_GAP, laid++ % per_row, &sw);
        if (desk_show_place(out, map->count + 5, page, sx + 5, top + row * (h + DESK_GAP) + 5,
                            sw - 10, h - 10, TILE_MINI) != 0)
            return -1;
    }
    return 0;
}
