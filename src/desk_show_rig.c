#include "desk_show_rig.h"

#include "desk_layout.h"
#include "desk_show_is_rig.h"
#include "desk_show_layout.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_slot.h"

#define MINI_H 64

int desk_show_rig(const struct show_map *map, int page, int x, int w, struct desk_layout *out) {
    if (desk_show_panel(out, page, x, DESK_LOWER_Y, w, DESK_LOWER_H, "COLOR DEL RIG", -1,
                        DESK_PANEL_NAMES_RUNNING) != 0)
        return -1;
    int x0 = x + DESK_PANEL_PAD, y0 = DESK_LOWER_Y + DESK_PANEL_TOP;
    int inner_w = w - 2 * DESK_PANEL_PAD, inner_h = DESK_LOWER_H - DESK_PANEL_TOP - DESK_PANEL_PAD;
    int one_row = (inner_w - (DESK_RIG_SLOTS - 1) * DESK_GAP) / DESK_RIG_SLOTS;
    int rows = one_row >= DESK_MINI_MIN_W ? 1 : 2;
    int per_row = (DESK_RIG_SLOTS + rows - 1) / rows;
    int h = rows == 1 ? MINI_H : (inner_h - DESK_GAP) / 2;
    int top = y0 + (inner_h - rows * h - (rows - 1) * DESK_GAP) / 2;
    int laid = 0;
    for (int i = 0; i < map->count && laid < DESK_RIG_SLOTS; i++) {
        if (!desk_show_is_rig(&map->control[i]))
            continue;
        int row = laid / per_row, sw;
        int sx = desk_show_slot(x0, inner_w, per_row, DESK_GAP, laid++ % per_row, &sw);
        if (desk_show_place(out, i, page, sx, top + row * (h + DESK_GAP), sw, h, TILE_MINI) != 0)
            return -1;
    }
    return 0;
}
