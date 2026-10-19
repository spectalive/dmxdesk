#include "desk_paint_hold.h"

#include <stdio.h>

#include "canvas_blend.h"
#include "desk_caption.h"
#include "desk_layout.h"
#include "desk_paint_keyline.h"
#include "desk_paint_text_at.h"
#include "desk_paint_text_centred.h"
#include "icon.h"
#include "icon_glyph.h"
#include "icon_paint_mid.h"

#define PILL "MANTENER"
#define PILL_H 18
#define PILL_PAD 8
#define GAP 4
#define OUTLINE 3

static int height_of(struct font *f) { return f ? font_height(f) : 15; }
static int width_of(struct font *f, const char *s) { return f ? font_width(f, s) : canvas_text_width(s, 3); }

void desk_paint_hold(struct canvas *c, const struct desk_control *ctl, const struct desk_placement *p,
                     const struct desk_fonts *fonts, enum desk_link link) {
    int live = ctl->enabled && link == DESK_LINK_READY;
    int capped = ctl->hold_progress >= 1000;
    int unresolved = ctl->hold_progress == -2;
    int running = ctl->kind == DESK_BURST && ctl->state == DESK_ON && ctl->enabled;
    int fog = p->tile == TILE_FOG;
    int armed = live && !capped;
    uint32_t tone = fog ? DESK_FOG_LINE : DESK_HOLD_LINE;
    uint32_t fill = running ? DESK_SELECTED_FILL : armed ? (fog ? DESK_FOG_FILL : DESK_HOLD_FILL) : DESK_GLASS;
    uint32_t line = unresolved ? DESK_WARN : armed ? tone : DESK_LINE;
    desk_paint_keyline(c, p->x, p->y, p->w, p->h, DESK_RADIUS, fill, line);
    if (ctl->pressed && !capped) {
        canvas_blend_round_rect(c, p->x, p->y, p->w, p->h, DESK_RADIUS, DESK_INK);
        canvas_blend_round_rect(c, p->x + OUTLINE, p->y + OUTLINE, p->w - 2 * OUTLINE, p->h - 2 * OUTLINE,
                                DESK_RADIUS - OUTLINE, fill);
    }
    uint32_t ink = running ? DESK_ACCENT_INK : armed ? DESK_INK : DESK_MUTED;
    uint32_t mark_ink = running ? DESK_ACCENT_INK : armed ? tone : DESK_LINE;
    // The hit's own mark, the bolt when the map gives none.
    int glyph = icon_glyph(ctl->icon);
    enum icon_id mark = glyph >= 0 ? (enum icon_id)glyph : ICON_BOLT;
    // The words under the name: the pill while it can fire, else why not.
    const char *foot = unresolved ? "apagado sin confirmar" : capped ? "suelta y vuelve"
                     : live ? NULL : "solo en el Mac";
    char length[24] = "";
    if (!foot && ctl->kind == DESK_BURST)
        snprintf(length, sizeof length, "m\xc3\xa1x %d s", (ctl->burst_ms + 500) / 1000);
    int counting = ctl->hold_progress >= 0;
    struct font *face = fonts->tile_s;
    int pitch = height_of(face), pill_w = width_of(fonts->section, PILL) + 2 * PILL_PAD;
    int foot_h = foot ? height_of(fonts->small) : PILL_H;
    int length_h = length[0] ? GAP + height_of(fonts->small) : 0;
    int px, py, tx, tw;
    if (p->w >= 2 * p->h) {
        // Wide: the mark at the left, the name over the pill and the length.
        int size = p->h >= ICON_MID_SIZE + 16 ? ICON_MID_SIZE : ICON_SIZE;
        int mx = p->x + 16, my = p->y + (p->h - size) / 2;
        if (size == ICON_MID_SIZE)
            icon_paint_mid(c, mark, mx, my, mark_ink);
        else
            icon_paint(c, mark, mx, my, mark_ink);
        tx = mx + size + 16;
        tw = p->x + p->w - 16 - tx;
        int top = p->y + (p->h - pitch - GAP - foot_h) / 2;
        desk_paint_text_at(c, face, tx, top, tw, ctl->label, ink);
        px = tx;
        py = top + pitch + GAP;
    } else {
        // A pad: the mark over the name over the pill, the length under it;
        // the 24 px mark when the 36 px one leaves the words no room.
        struct desk_caption_lines name;
        desk_caption_fit(face, p->w - 16, ctl->label, &name);
        int words = name.lines * pitch + GAP + foot_h + length_h;
        int size = words + GAP + ICON_MID_SIZE <= p->h - 8 ? ICON_MID_SIZE : ICON_SIZE;
        int top = p->y + (p->h - size - GAP - words) / 2;
        if (size == ICON_MID_SIZE)
            icon_paint_mid(c, mark, p->x + (p->w - size) / 2, top, mark_ink);
        else
            icon_paint(c, mark, p->x + (p->w - size) / 2, top, mark_ink);
        top += size + GAP;
        for (int i = 0; i < name.lines; i++)
            desk_paint_text_centred(c, face, p->x + 8, top + i * pitch, p->w - 16, pitch, name.line[i], ink);
        tx = p->x + 4;
        tw = p->w - 8;
        px = p->x + (p->w - pill_w) / 2;
        py = top + name.lines * pitch + GAP;
    }
    if (foot) {
        if (p->w >= 2 * p->h)
            desk_paint_text_at(c, fonts->small, tx, py, tw, foot, unresolved ? DESK_WARN : DESK_MUTED);
        else
            desk_paint_text_centred(c, fonts->small, tx, py, tw, foot_h, foot, unresolved ? DESK_WARN : DESK_MUTED);
    } else {
        // Light text on the violet pill, dark on the brighter teal.
        canvas_blend_round_rect(c, px, py, pill_w, PILL_H, PILL_H / 2, tone);
        desk_paint_text_centred(c, fonts->section, px, py, pill_w, PILL_H, PILL, fog ? DESK_GLASS : DESK_ACCENT_INK);
        // The length after the pill on a wide pad, under it on a square one;
        // the countdown bar takes its place while the finger is down.
        if (length[0] && !counting) {
            if (p->w >= 2 * p->h)
                desk_paint_text_at(c, fonts->small, px + pill_w + 8, py + (PILL_H - height_of(fonts->small)) / 2,
                                   tw - pill_w - 8, length, running ? DESK_ACCENT_INK : DESK_MUTED);
            else
                desk_paint_text_centred(c, fonts->small, tx, py + PILL_H + GAP, tw, height_of(fonts->small),
                                        length, running ? DESK_ACCENT_INK : DESK_MUTED);
        }
    }
    // While the finger is down a thin bar counts the cap down.
    if (counting) {
        int span = p->w - 16;
        int used = span * (ctl->hold_progress > 1000 ? 1000 : ctl->hold_progress) / 1000;
        canvas_fill_rect(c, p->x + 8, p->y + p->h - 8, span, 3, DESK_LINE);
        canvas_fill_rect(c, p->x + 8, p->y + p->h - 8, span - used, 3, DESK_INK);
    }
}
