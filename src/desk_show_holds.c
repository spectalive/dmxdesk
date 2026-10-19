#include "desk_show_holds.h"

#include "desk_layout.h"
#include "desk_show_accent_tile.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_slot.h"

#define PER_ROW 3
// No row of pads is laid shorter than a finger.
#define PAD_MIN_H 48

int desk_show_holds(const struct show_map *map, const struct map_section *accents, int section, int page,
                    struct desk_layout *out) {
    if (desk_show_panel(out, page, DESK_HITS_X, DESK_HITS_Y, DESK_HITS_W, 0,
                        "EFECTOS \xc2\xb7 MIENTRAS PULSES", section, DESK_PANEL_PLAIN) != 0)
        return -1;
    int room = DESK_HITS_END - DESK_HITS_TOP;
    int most = (room + DESK_GAP) / (PAD_MIN_H + DESK_GAP) * PER_ROW;
    int n = accents->count < most ? accents->count : most;
    int rows = (n + PER_ROW - 1) / PER_ROW, last = n - PER_ROW * (rows - 1);
    int wide = rows > 1 && last == 1;
    int grid_rows = wide ? rows - 1 : rows;
    int grid_room = wide ? room - DESK_HOLD_WIDE_H - DESK_GAP : room;
    int laid = 0;
    for (int fog = 0; fog <= 1; fog++)
        for (int k = 0; k < n; k++) {
            const struct map_control *c = &map->control[accents->first + k];
            enum desk_tile tile = desk_show_accent_tile(c);
            if ((tile == TILE_FOG) != fog)
                continue;
            int slot = laid++, row = slot / PER_ROW;
            int in_row = row == rows - 1 ? last : PER_ROW;
            int w, x = desk_show_slot(DESK_HITS_X, DESK_HITS_W, in_row, DESK_GAP, slot % PER_ROW, &w);
            int h = DESK_HOLD_WIDE_H, y = DESK_HITS_END - DESK_HOLD_WIDE_H;
            if (!wide || row < grid_rows)
                y = desk_show_slot(DESK_HITS_TOP, grid_room, grid_rows, DESK_GAP, row, &h);
            if (desk_show_place(out, accents->first + k, page, x, y, w, h, tile) != 0)
                return -1;
        }
    return 0;
}
