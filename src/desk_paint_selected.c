#include "desk_paint_selected.h"

#include "desk_layout.h"
#include "desk_paint_keyline.h"

void desk_paint_selected(struct canvas *c, int x, int y, int w, int h, int r) {
    desk_paint_keyline(c, x, y, w, h, r, DESK_SELECTED_FILL, DESK_SELECTED_LINE);
    canvas_fill_rect(c, x + r, y, w - 2 * r, 1, DESK_SELECTED_TOP);
}
