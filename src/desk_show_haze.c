#include "desk_show_haze.h"

#include "desk_layout.h"
#include "desk_show_panel.h"
#include "desk_show_place.h"
#include "desk_show_slot.h"

int desk_show_haze(const struct map_section *haze, int section, int haze_off,
                   int page, int x, struct desk_layout *out) {
    if (desk_show_panel(out, page, x, DESK_HAZE_Y, DESK_HAZE_W, DESK_HAZE_H, haze->title, section,
                        DESK_PANEL_PLAIN) != 0)
        return -1;
    int x0 = x + DESK_PANEL_PAD, y0 = DESK_HAZE_Y + DESK_PANEL_TOP;
    int w = DESK_HAZE_W - 2 * DESK_PANEL_PAD, h = DESK_HAZE_H - DESK_PANEL_TOP - DESK_PANEL_PAD;
    if (haze_off >= 0) {
        if (desk_show_place(out, haze_off, page, x0, y0, DESK_HAZE_OFF_W, h, TILE_SEGMENT) != 0)
            return -1;
        x0 += DESK_HAZE_OFF_W + DESK_GAP;
        w -= DESK_HAZE_OFF_W + DESK_GAP;
    }
    int n = haze->count, rows = n > 1 ? 2 : 1, cols = (n + rows - 1) / rows;
    for (int k = 0; k < n; k++) {
        int row = k / cols, in_row = row == rows - 1 ? n - cols * (rows - 1) : cols;
        int sw, sx = desk_show_slot(x0, w, in_row, DESK_GAP, k % cols, &sw);
        int sh, sy = desk_show_slot(y0, h, rows, DESK_GAP, row, &sh);
        if (desk_show_place(out, haze->first + k, page, sx, sy, sw, sh, TILE_SEGMENT) != 0)
            return -1;
    }
    return 0;
}
