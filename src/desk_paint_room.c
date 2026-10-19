#include "desk_paint_room.h"

#include <stdio.h>

#include "desk_caption.h"
#include "desk_layout.h"
#include "desk_panel_holds.h"
#include "desk_paint_text_at.h"

static int width_of(struct font *f, const char *s) { return f ? font_width(f, s) : canvas_text_width(s, 3); }
static int height_of(struct font *f) { return f ? font_height(f) : 15; }

void desk_paint_room(struct canvas *c, const struct desk_model *m, const struct desk_heading *h,
                     const struct desk_fonts *fonts) {
    // The block ends 12 px short of the first state tile.
    const struct desk_control *on = NULL;
    int unknown = m->link != DESK_LINK_READY, left = h->x + h->w;
    for (int i = 0; i < m->layout.placements; i++) {
        const struct desk_placement *p = &m->layout.placement[i];
        if (!desk_panel_holds(h, p) || p->tile != TILE_STATE || p->control < 0 || p->control >= m->count)
            continue;
        if (p->x < left)
            left = p->x;
        const struct desk_control *ctl = &m->control[p->control];
        if (ctl->state == DESK_UNKNOWN)
            unknown = 1;
        else if (ctl->enabled && ctl->state == DESK_ON)
            on = ctl;
    }
    // Without the master, nothing it said still holds.
    if (m->link != DESK_LINK_READY)
        on = NULL;
    int x = h->x + DESK_PANEL_PAD, w = left - 12 - x;
    if (w <= 0)
        return;
    const char *name = on ? on->label : unknown ? "?" : "\xe2\x80\x94";
    uint32_t ink = on ? DESK_INK : DESK_MUTED;
    // The largest face the name keeps to one line in, then the readout face
    // on two lines, then the tile face.
    struct desk_caption_lines lines = { .lines = 1 };
    struct font *face = fonts->big;
    if (width_of(fonts->big, name) > w) {
        face = fonts->value;
        if (width_of(fonts->value, name) > w) {
            desk_caption_fit(fonts->value, w, name, &lines);
            if (lines.cut) {
                face = fonts->tile;
                lines.lines = 1;
            }
        }
    }
    if (lines.lines == 1)
        snprintf(lines.line[0], sizeof lines.line[0], "%s", name);
    const char *detail = on ? on->detail : "";
    int top = h->y + DESK_PANEL_TOP - 4;
    int bottom = h->y + h->h - DESK_PANEL_PAD - (detail[0] ? height_of(fonts->small) + 2 : 0);
    int pitch = lines.lines > 1 ? height_of(face) - 6 : height_of(face);
    int y = top + (bottom - top - lines.lines * pitch) / 2;
    for (int i = 0; i < lines.lines; i++)
        desk_paint_text_at(c, face, x, y + i * pitch, w, lines.line[i], ink);
    if (detail[0])
        desk_paint_text_at(c, fonts->small, x, bottom, w, detail, DESK_MUTED);
}
