#include "desk_paint_room_picture.h"

#include "canvas_blend.h"
#include "desk_layout.h"
#include "icon_glyph.h"
#include "icon_paint_hero.h"

#define BARS_H 40
#define BARS_GAP 8
#define MARK_GAP 16

void desk_paint_room_picture(struct canvas *c, const struct desk_control *on, int x, int y, int w, int h) {
    int mark = icon_glyph(on->icon);
    int marked = mark >= 0 && ICON_HERO_OF[mark] >= 0;
    int n = on->swatches > DESK_MAX_SWATCHES ? DESK_MAX_SWATCHES : on->swatches;
    int block = (marked ? ICON_HERO_SIZE : 0) + (marked && n ? MARK_GAP : 0) + (n ? BARS_H : 0);
    int top = y + (h - block) / 2;
    if (marked) {
        icon_paint_hero(c, (enum icon_id)mark, x + (w - ICON_HERO_SIZE) / 2, top, DESK_ACCENT_INK);
        top += ICON_HERO_SIZE + MARK_GAP;
    }
    if (!n)
        return;
    // One colour is a wide bar, a few are slimmer; all of them span alike.
    int bar = n <= 2 ? 32 : 16;
    int row = n * bar + (n - 1) * BARS_GAP;
    if (row > w) {
        bar = (w - (n - 1) * BARS_GAP) / n;
        row = n * bar + (n - 1) * BARS_GAP;
    }
    for (int i = 0; i < n; i++)
        canvas_blend_round_rect(c, x + (w - row) / 2 + i * (bar + BARS_GAP), top, bar, BARS_H, bar / 2 < 8 ? bar / 2 : 8,
                                on->swatch[i] | 0xFF000000u);
}
