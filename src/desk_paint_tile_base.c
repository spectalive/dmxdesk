#include "desk_paint_tile_base.h"

#include "desk_layout.h"
#include "desk_paint_keyline.h"

#define OUTLINE 3

uint32_t desk_paint_tile_base(struct canvas *c, const struct desk_control *ctl,
                              const struct desk_placement *p, enum desk_link link) {
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = !ctl->enabled ? DESK_GLASS : on ? DESK_ACCENT : DESK_TILE;
    uint32_t line = on ? fill : ctl->enabled ? DESK_LINE : DESK_RAISED;
    desk_paint_keyline(c, p->x, p->y, p->w, p->h, DESK_RADIUS, fill, line);
    // A finger draws an ink outline and nothing else: the fill is the show's.
    if (ctl->pressed) {
        canvas_round_rect(c, p->x, p->y, p->w, p->h, DESK_RADIUS, DESK_INK);
        canvas_round_rect(c, p->x + OUTLINE, p->y + OUTLINE, p->w - 2 * OUTLINE, p->h - 2 * OUTLINE,
                          DESK_RADIUS - OUTLINE, fill);
    }
    if (ctl->pending)
        canvas_round_rect(c, p->x + p->w - 18, p->y + 8, 10, 10, 5, DESK_INK);
    if (!ctl->enabled || link != DESK_LINK_READY)
        return DESK_MUTED;
    return on ? DESK_ACCENT_INK : DESK_INK;
}
