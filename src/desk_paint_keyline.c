#include "desk_paint_keyline.h"

#include "canvas_blend.h"

void desk_paint_keyline(struct canvas *c, int x, int y, int w, int h, int r, uint32_t fill,
                        uint32_t line) {
    canvas_blend_round_rect(c, x, y, w, h, r, line);
    if (line != fill)
        canvas_blend_round_rect(c, x + 1, y + 1, w - 2, h - 2, r - 1, fill);
}
