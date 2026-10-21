#include "desk_paint_room.h"

#include <stdio.h>

#include "canvas_blend.h"
#include "desk_caption.h"
#include "desk_layout.h"
#include "desk_paint_keyline.h"
#include "desk_paint_room_status.h"
#include "desk_paint_text_at.h"
#include "desk_text_upper.h"

static int width_of(struct font *f, const char *s) { return f ? font_width(f, s) : canvas_text_width(s, 3); }
static int height_of(struct font *f) { return f ? font_height(f) : 15; }

#define INSET 24
#define DOT 8
#define STATUS_W 214

void desk_paint_room(struct canvas *c, const struct desk_model *m, const struct desk_heading *h,
                     const struct desk_fonts *fonts) {
    // The room's states are the state tiles on this view.
    const struct desk_control *on = NULL;
    int unknown = m->link != DESK_LINK_READY;
    for (int i = 0; i < m->layout.placements; i++) {
        const struct desk_placement *p = &m->layout.placement[i];
        if (p->page != m->page || p->bank != m->bank || p->tile != TILE_STATE || p->control < 0 ||
            p->control >= m->count)
            continue;
        const struct desk_control *ctl = &m->control[p->control];
        if (ctl->state == DESK_UNKNOWN)
            unknown = 1;
        else if (ctl->enabled && ctl->state == DESK_ON)
            on = ctl;
    }
    // Without the master, nothing it said still holds.
    if (m->link != DESK_LINK_READY)
        on = NULL;
    if (on)
        desk_paint_keyline(c, h->x, h->y, h->w, h->h, DESK_PANEL_RADIUS, DESK_HERO_FILL, DESK_ACCENT);
    else
        canvas_blend_round_rect(c, h->x, h->y, h->w, h->h, DESK_PANEL_RADIUS, DESK_PANEL);
    // The heading after a dot that says whether the master answers.
    int hx = h->x + INSET, hy = h->y + 18;
    canvas_blend_round_rect(c, hx, hy + (height_of(fonts->section) - DOT) / 2, DOT, DOT, DOT / 2,
                            m->link == DESK_LINK_READY ? DESK_OK : DESK_WARN);
    char cap[96];
    desk_text_upper(cap, sizeof cap, h->text);
    desk_paint_text_at(c, fonts->section, hx + DOT + 8, hy, h->w - 2 * INSET - DOT - 8, cap,
                       on ? DESK_HERO_SOFT : DESK_MUTED);
    int right = h->x + h->w - INSET - STATUS_W - 16;
    desk_paint_room_status(c, m, right + 16, h->y + 25, STATUS_W, fonts);
    int x = h->x + INSET, w = right - x;
    if (w <= 0)
        return;
    // A bare dash left the largest panel on the desk looking broken
    // (2026-10-19), so an idle room says so and where to choose one.
    int idle = !on && !unknown;
    const char *name = on ? on->label : unknown ? "?" : "Sin escena activa";
    // The largest face the name keeps to one line in, then the tempo's, then
    // the readout face on two lines, then the tile face.
    struct desk_caption_lines lines = { .lines = 1 };
    struct font *face = h->h < 160 ? fonts->big : fonts->huge;
    if (width_of(face, name) > w)
        face = fonts->big;
    if (width_of(face, name) > w) {
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
    // The detail in the readout face while it fits, else the tile face.
    const char *detail = on ? on->detail : idle ? "Selecciona un modo" : "";
    struct font *quiet = h->h < 160 ? fonts->tile :
                         width_of(fonts->value, detail) <= w ? fonts->value : fonts->tile;
    int top = hy + height_of(fonts->section) + 8, bottom = h->y + h->h - INSET;
    int pitch = lines.lines > 1 ? height_of(face) - 6 : height_of(face);
    int block = lines.lines * pitch + (detail[0] ? 4 + height_of(quiet) : 0);
    int y = top + (bottom - top - block) / 2;
    for (int i = 0; i < lines.lines; i++)
        desk_paint_text_at(c, face, x, y + i * pitch, w, lines.line[i], on ? DESK_ACCENT_INK : DESK_MUTED);
    if (detail[0])
        desk_paint_text_at(c, quiet, x, y + lines.lines * pitch + 4, w, detail, DESK_HERO_SOFT);
}
