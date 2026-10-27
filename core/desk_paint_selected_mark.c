#include "desk_paint_selected_mark.h"

#include "canvas_blend.h"
#include "desk_layout.h"

#define MARK_H 4
#define MARK_INSET 6

void desk_paint_selected_mark(struct canvas *c, int x, int y, int w, int h) {
    int bar = w / 4;
    if (bar < 16)
        bar = 16;
    if (bar > 32)
        bar = 32;
    canvas_blend_round_rect(c, x + (w - bar) / 2, y + h - MARK_INSET - MARK_H, bar, MARK_H, MARK_H / 2,
                            DESK_ACCENT);
}
