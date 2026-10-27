#include "desk_paint_tile_base.h"

#include "canvas_blend.h"
#include "desk_layout.h"
#include "desk_paint_keyline.h"

#define OUTLINE 3
#define RING 2

uint32_t desk_paint_tile_base(struct canvas *c, const struct desk_control *ctl,
                              const struct desk_placement *p, enum desk_link link) {
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = !ctl->enabled ? DESK_GLASS : on ? DESK_SELECTED_FILL : DESK_TILE;
    // Running, a pick is ringed in the selected line round the frosted fill,
    // as a colour pick is: its caption sits too low for the accent bar.
    if (on) {
        canvas_blend_round_rect(c, p->x, p->y, p->w, p->h, DESK_RADIUS, DESK_SELECTED_LINE);
        canvas_blend_round_rect(c, p->x + RING, p->y + RING, p->w - 2 * RING, p->h - 2 * RING,
                                DESK_RADIUS - RING, DESK_SELECTED_FILL);
    } else {
        desk_paint_keyline(c, p->x, p->y, p->w, p->h, DESK_RADIUS, fill, ctl->enabled ? DESK_LINE : DESK_RAISED);
    }
    // A finger draws an ink outline and nothing else: the fill is the show's.
    if (ctl->pressed) {
        canvas_blend_round_rect(c, p->x, p->y, p->w, p->h, DESK_RADIUS, DESK_INK);
        canvas_blend_round_rect(c, p->x + OUTLINE, p->y + OUTLINE, p->w - 2 * OUTLINE, p->h - 2 * OUTLINE,
                          DESK_RADIUS - OUTLINE, fill);
    }
    if (ctl->pending)
        canvas_blend_round_rect(c, p->x + p->w - 18, p->y + 8, 10, 10, 5, DESK_INK);
    if (!ctl->enabled || link != DESK_LINK_READY)
        return DESK_MUTED;
    return on ? DESK_ACCENT_INK : DESK_INK;
}
