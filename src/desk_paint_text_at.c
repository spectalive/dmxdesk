#include "desk_paint_text_at.h"

void desk_paint_text_at(struct canvas *c, struct font *f, int x, int y, int max_w, const char *s,
                        uint32_t col) {
    if (f)
        font_draw_fit(f, c, x, y + font_baseline(f), max_w, s, col);
    else
        canvas_text(c, x, y, s, 3, col);
}
