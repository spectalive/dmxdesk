#include "desk_paint_text_centred.h"

void desk_paint_text_centred(struct canvas *c, struct font *f, int x, int y, int w, int h,
                             const char *s, uint32_t col) {
    int width = f ? font_width(f, s) : canvas_text_width(s, 3);
    if (width > w)
        width = w;
    int line = f ? font_height(f) : 15;
    int tx = x + (w - width) / 2, ty = y + (h - line) / 2;
    if (f)
        font_draw_fit(f, c, tx, ty + font_baseline(f), w, s, col);
    else
        canvas_text(c, tx, ty, s, 3, col);
}
