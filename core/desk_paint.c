#include "desk_paint.h"

#include <stdio.h>
#include <string.h>

#include "canvas_blend.h"
#include "desk_caption.h"
#include "desk_colour_dim.h"
#include "desk_layout.h"
#include "desk_mark_dark.h"
#include "desk_master_track.h"
#include "desk_master_metrics.h"
#include "desk_view.h"
#include "desk_pager_bank_caption.h"
#include "desk_paint_families.h"
#include "desk_paint_hold.h"
#include "desk_paint_keyline.h"
#include "desk_paint_number.h"
#include "desk_paint_room.h"
#include "desk_paint_selected.h"
#include "desk_paint_selected_mark.h"
#include "desk_paint_tabs.h"
#include "desk_paint_trajectory.h"
#include "desk_panel_running.h"
#include "desk_panel_title.h"
#include "desk_rig_name.h"
#include "desk_tab.h"
#include "desk_text_upper.h"
#include "icon.h"
#include "icon_glyph.h"
#include "icon_paint_mid.h"

// Alpha stays 0xFF everywhere, as in rescue_paint: the framebuffer is added
// as XRGB8888 and a zero top byte has already once turned the whole panel
// into a washed-out ghost.
#define BORDER 3
#define DISC 28

struct rect { int x, y, w, h; };

static struct rect of(const struct desk_placement *p) {
    struct rect r = { p->x, p->y, p->w, p->h };
    return r;
}

// ---- text helpers: every string is drawn with a font when there is one
// and with the 3x5 glyphs when there is not.

static int text_w(struct font *f, const char *s) {
    return f ? font_width(f, s) : canvas_text_width(s, 3);
}

static int text_h(struct font *f) { return f ? font_height(f) : 15; }

static void text_at(struct canvas *c, struct font *f, int x, int y, int max_w, const char *s,
                    uint32_t col) {
    if (f)
        font_draw_fit(f, c, x, y + font_baseline(f), max_w, s, col);
    else
        canvas_text(c, x, y, s, 3, col);
}

static void centred(struct canvas *c, struct font *f, int x, int y, int w, int h, const char *s,
                    uint32_t col) {
    int width = text_w(f, s);
    if (width > w)
        width = w;
    text_at(c, f, x + (w - width) / 2, y + (h - text_h(f)) / 2, w, s, col);
}



// The caption on one or two measured lines, centred on `mid_y`.
static void caption(struct canvas *c, struct font *f, int x, int w, int mid_y, const char *text,
                    uint32_t col) {
    struct desk_caption_lines lines;
    desk_caption_fit(f, w, text, &lines);
    int pitch = text_h(f) + 3;
    int top = mid_y - (lines.lines * pitch) / 2;
    for (int i = 0; i < lines.lines; i++)
        centred(c, f, x, top + i * pitch, w, pitch, lines.line[i], col);
}

void desk_paint_section(struct canvas *c, const struct desk_fonts *fonts, int x, int y, int w,
                        const char *title) {
    char cap[96];
    desk_text_upper(cap, sizeof cap, title);
    text_at(c, fonts->section, x, y + 4, w, cap, DESK_MUTED);
    int tw = text_w(fonts->section, cap);
    if (tw > w)
        tw = w;
    if (tw + 16 < w)
        canvas_fill_rect(c, x + tw + 12, y + 12, w - tw - 12, 1, DESK_LINE);
}

// ---- tiles

// A pressed tile gets an ink outline and nothing else. The fill is the
// show's to give: the tile lights when the master says the function runs,
// and a finger must not paint the selected look, since that means on.
static void press_outline(struct canvas *c, struct rect r, uint32_t fill) {
    canvas_blend_round_rect(c, r.x, r.y, r.w, r.h, DESK_RADIUS, DESK_INK);
    canvas_blend_round_rect(c, r.x + BORDER, r.y + BORDER, r.w - 2 * BORDER, r.h - 2 * BORDER,
                      DESK_RADIUS - BORDER, fill);
}

// The words a tile carries under its name, one look across every tile: a
// disabled one says why in plain words, an unknown one says so, an idle one
// shows the show's own detail when there is room for it.
static const char *state_line(const struct desk_control *ctl, enum desk_link link) {
    if (!ctl->enabled) {
        if (strncmp(ctl->reason, "held", 4) == 0)
            return "solo en el Mac";
        if (strcmp(ctl->reason, "master timeout missing") == 0)
            return "actualiza QLC+";
        if (strcmp(ctl->reason, "master priority missing") == 0)
            return "revisa Flash en QLC+";
        return "no disponible";
    }
    if (link != DESK_LINK_READY)
        return "";
    if (ctl->state == DESK_UNKNOWN)
        return "?";
    return ctl->detail;
}

static uint32_t tile_fill(const struct desk_control *ctl) {
    if (!ctl->enabled)
        return DESK_GLASS;
    return ctl->state == DESK_ON ? DESK_SELECTED_FILL : DESK_TILE;
}

// A tile is its fill inside a 1 px keyline, dimmer when it is disabled;
// running, it wears the selected look and its accent bar.
static void tile_frame(struct canvas *c, struct rect r, int radius, const struct desk_control *ctl,
                       uint32_t fill) {
    if (fill == DESK_SELECTED_FILL) {
        desk_paint_selected(c, r.x, r.y, r.w, r.h, radius);
        desk_paint_selected_mark(c, r.x, r.y, r.w, r.h);
        return;
    }
    desk_paint_keyline(c, r.x, r.y, r.w, r.h, radius, fill, ctl->enabled ? DESK_LINE : DESK_RAISED);
}

// The tile's mark: the map's glyph drawn as the desk's own vector icon, in
// the tile's ink. A glyph the desk does not know draws nothing, and a mark
// always gives way to words: it is only placed where the caption keeps one
// line beside or under it.
#define MARK ICON_SIZE
#define MARK_GAP 8

static void pending_mark(struct canvas *c, const struct desk_control *ctl, struct rect r) {
    if (ctl->pending)
        canvas_blend_round_rect(c, r.x + r.w - 18, r.y + 8, 10, 10, 5, DESK_INK);
}

// A 200x64 button: the name, and under it the one line that matters.
static void paint_cue(struct canvas *c, const struct desk_control *ctl,
                      const struct desk_placement *p, const struct desk_fonts *fonts,
                      enum desk_link link) {
    struct rect r = of(p);
    uint32_t fill = tile_fill(ctl);
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t ink = on ? DESK_ACCENT_INK : DESK_INK;
    tile_frame(c, r, DESK_RADIUS, ctl, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    const char *under = state_line(ctl, link);
    // The mark sits at the left, the words centred in what remains.
    int mark = icon_glyph(ctl->icon);
    int tx = r.x + 10, tw = r.w - 20;
    if (mark >= 0) {
        icon_paint(c, (enum icon_id)mark, r.x + 12, r.y + (r.h - MARK) / 2, ink);
        tx = r.x + 12 + MARK + MARK_GAP;
        tw = r.x + r.w - 10 - tx;
    }
    // The name in the small tile face, one size across a row of four hooks,
    // on one line or two.
    struct font *face = fonts->tile_s;
    struct desk_caption_lines lines;
    desk_caption_fit(face, tw, ctl->label, &lines);
    int keep_line = under[0] && (!ctl->enabled || ctl->state == DESK_UNKNOWN);
    if (under[0] && lines.lines <= 1) {
        centred(c, face, tx, r.y + 10, tw, text_h(face) + 4, ctl->label, ink);
        centred(c, fonts->small, tx, r.y + r.h - 12 - text_h(fonts->small) - 2, tw,
                text_h(fonts->small) + 4, under, on ? DESK_ACCENT_INK : DESK_MUTED);
    } else if (keep_line) {
        // Why it is dead or unknown stays, so the name takes two small lines.
        desk_caption_fit(fonts->tile_s, tw, ctl->label, &lines);
        int pitch = text_h(fonts->tile_s) + 1, foot = text_h(fonts->small);
        int top = r.y + (r.h - lines.lines * pitch - 2 - foot) / 2;
        for (int i = 0; i < lines.lines; i++)
            centred(c, fonts->tile_s, tx, top + i * pitch, tw, pitch, lines.line[i], ink);
        centred(c, fonts->small, tx, top + lines.lines * pitch + 2, tw, foot, under,
                on ? DESK_ACCENT_INK : DESK_MUTED);
    } else {
        caption(c, face, tx, tw, r.y + r.h / 2, ctl->label, ink);
    }
    pending_mark(c, ctl, r);
}

// A pick: its colours as a square (stripes for a mix) over its name, the
// whole tile the target; the square takes what the name leaves of the tile,
// at most 64 px. A pick with no colour wears its family's mark in a disc's
// place, or, without one, has its name alone, larger; running, it wears the
// selected look like any other tile.
#define SQUARE_MAX 64

static void paint_swatch(struct canvas *c, const struct desk_control *ctl,
                         const struct desk_placement *p, const struct desk_fonts *fonts,
                         enum desk_link link) {
    struct rect r = of(p);
    int on = ctl->state == DESK_ON && ctl->enabled;
    int ringed = on && ctl->swatches > 0;
    uint32_t fill = tile_fill(ctl);
    // A coloured pick runs ringed in the selected line round the frosted
    // fill, so its own colour stays whole and visible.
    if (ringed) {
        canvas_blend_round_rect(c, r.x, r.y, r.w, r.h, DESK_RADIUS, DESK_SELECTED_LINE);
        canvas_blend_round_rect(c, r.x + BORDER, r.y + BORDER, r.w - 2 * BORDER, r.h - 2 * BORDER,
                          DESK_RADIUS - BORDER, DESK_SELECTED_FILL);
        fill = DESK_SELECTED_FILL;
    } else {
        tile_frame(c, r, DESK_RADIUS, ctl, fill);
    }
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = on && !ringed ? DESK_ACCENT_INK : DESK_INK;
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    pending_mark(c, ctl, r);
    int cx = r.x + r.w / 2, cy = r.y + 10;
    if (ctl->swatches == 0) {
        // No colour: the family's mark stands where the disc would, over the
        // same words; without one, the name alone, larger.
        int mark = icon_glyph(ctl->icon);
        if (mark < 0) {
            caption(c, fonts->tile, r.x + 8, r.w - 16, r.y + r.h / 2, ctl->label, ink);
            return;
        }
        icon_paint(c, (enum icon_id)mark, cx - MARK / 2, cy + (DISC - MARK) / 2, ink);
        if (ctl->detail[0]) {
            centred(c, fonts->tile_s, r.x + 8, r.y + DISC + 12, r.w - 16, 20, ctl->label, ink);
            centred(c, fonts->small, r.x + 8, r.y + DISC + 32, r.w - 16, 18, ctl->detail,
                    on ? DESK_ACCENT_INK : DESK_MUTED);
        } else {
            caption(c, fonts->tile_s, r.x + 8, r.w - 16, r.y + DISC + 12 + (r.h - DISC - 12) / 2,
                    ctl->label, ink);
        }
        return;
    }
    // The words first: their lines decide how much height the square keeps.
    struct desk_caption_lines name;
    desk_caption_fit(fonts->tile_s, r.w - 16, ctl->label, &name);
    int pitch = text_h(fonts->tile_s) + 3, small_pitch = text_h(fonts->small) + 2;
    int words = name.lines * pitch + (ctl->detail[0] ? small_pitch : 0);
    int side = r.h - 8 - 6 - words - 6;
    if (side > SQUARE_MAX)
        side = SQUARE_MAX;
    if (side > r.w - 24)
        side = r.w - 24;
    int top = r.y + (r.h - side - 6 - words) / 2;
    int sx0 = cx - side / 2;
    int old_x = c->clip_x, old_y = c->clip_y, old_w = c->clip_w, old_h = c->clip_h;
    int n = ctl->swatches > DESK_MAX_SWATCHES ? DESK_MAX_SWATCHES : ctl->swatches;
    for (int i = 0; i < n; i++) {
        int sx = sx0 + i * side / n;
        int sw = (i + 1) * side / n - i * side / n;
        int cx0 = sx, cy0 = top, cw = sw, ch = side;
        if (old_w > 0) {
            int x1 = cx0 > old_x ? cx0 : old_x, y1 = cy0 > old_y ? cy0 : old_y;
            int x2 = cx0 + cw < old_x + old_w ? cx0 + cw : old_x + old_w;
            int y2 = cy0 + ch < old_y + old_h ? cy0 + ch : old_y + old_h;
            cx0 = x1; cy0 = y1; cw = x2 > x1 ? x2 - x1 : 0; ch = y2 > y1 ? y2 - y1 : 0;
        }
        if (cw <= 0 || ch <= 0)
            continue;
        canvas_set_clip(c, cx0, cy0, cw, ch);
        canvas_blend_round_rect(c, sx0, top, side, side, 8, ctl->swatch[i]);
    }
    if (old_w > 0)
        canvas_set_clip(c, old_x, old_y, old_w, old_h);
    else
        canvas_clear_clip(c);
    // Black and near-black squares need a keyline to exist on the tile.
    uint32_t first = ctl->swatch[0];
    int brightest = (first >> 16 & 0xFF) > (first >> 8 & 0xFF) ? (first >> 16 & 0xFF) : (first >> 8 & 0xFF);
    if (brightest < (int)(first & 0xFF))
        brightest = first & 0xFF;
    if (brightest < 0x20)
        canvas_blend_round_rect(c, sx0 - 1, top - 1, side + 2, side + 2, 9, DESK_MUTED),
        canvas_blend_round_rect(c, sx0, top, side, side, 8, first);
    int y = top + side + 6;
    for (int i = 0; i < name.lines; i++)
        centred(c, fonts->tile_s, r.x + 8, y + i * pitch, r.w - 16, pitch, name.line[i], ink);
    // A paired look says what the rest of the rig does: "Resto Azul".
    if (ctl->detail[0])
        centred(c, fonts->small, r.x + 8, y + name.lines * pitch, r.w - 16, small_pitch, ctl->detail,
                DESK_MUTED);
}

// A state under SALA AHORA: its mark over its name. The hero carries the
// running detail; the tile uses its second line only for a disabled or unknown
// state. Narrow two-line names get a smaller mark instead of losing it.
static void paint_state(struct canvas *c, const struct desk_control *ctl,
                        const struct desk_placement *p, const struct desk_fonts *fonts,
                        enum desk_link link) {
    struct rect r = of(p);
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = tile_fill(ctl);
    tile_frame(c, r, DESK_RADIUS, ctl, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = on ? DESK_ACCENT_INK : DESK_INK;
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    int unknown = link == DESK_LINK_READY && ctl->state == DESK_UNKNOWN;
    const char *under = !ctl->enabled || unknown ? state_line(ctl, link) : "";
    struct desk_caption_lines name, more;
    desk_caption_fit(fonts->section, r.w - 8, ctl->label, &name);
    int lines = under[0] ? desk_caption_fit(fonts->tiny, r.w - 8, under, &more) : 0;
    int name_pitch = text_h(fonts->section) + 2, more_pitch = text_h(fonts->tiny) + 1;
    int words = name.lines * name_pitch + (lines ? 4 + lines * more_pitch : 0);
    int mark = icon_glyph(ctl->icon);
    int margin = r.w < 80 ? 12 : 24;
    int mark_size = mark >= 0 && words + ICON_MID_SIZE + MARK_GAP <= r.h - margin ? ICON_MID_SIZE
                  : mark >= 0 && words + ICON_SIZE + MARK_GAP <= r.h - margin ? ICON_SIZE : 0;
    int y = r.y + (r.h - words - (mark_size ? mark_size + MARK_GAP : 0)) / 2;
    if (mark_size) {
        if (mark_size == ICON_MID_SIZE)
            icon_paint_mid(c, (enum icon_id)mark, r.x + (r.w - mark_size) / 2, y, ink);
        else
            icon_paint(c, (enum icon_id)mark, r.x + (r.w - mark_size) / 2, y, ink);
        y += mark_size + MARK_GAP;
    }
    for (int i = 0; i < name.lines; i++)
        centred(c, fonts->section, r.x + 4, y + i * name_pitch, r.w - 8, name_pitch, name.line[i], ink);
    y += name.lines * name_pitch + 4;
    // A qualification that keeps someone from a mistake reads as loud as the name.
    uint32_t more_ink = on ? DESK_ACCENT_INK : ctl->enabled && !unknown ? DESK_INK : DESK_MUTED;
    for (int i = 0; i < lines; i++)
        centred(c, fonts->tiny, r.x + 4, y + i * more_pitch, r.w - 8, more_pitch, more.line[i], more_ink);
    pending_mark(c, ctl, r);
}

// One option of the ambient selector: the running rhythm wears the selected
// look; so does OFF when the master says no rhythm runs, since then it is
// the one chosen.
static void paint_segment(struct canvas *c, const struct desk_control *ctl,
                          const struct desk_placement *p, const struct desk_fonts *fonts,
                          enum desk_link link, int none_running) {
    struct rect r = of(p);
    int on = ctl->kind == DESK_HAZE_OFF ? none_running : ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = !ctl->enabled ? DESK_GLASS : on ? DESK_SELECTED_FILL : DESK_TILE;
    tile_frame(c, r, 10, ctl, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = on ? DESK_ACCENT_INK : DESK_INK;
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    if (ctl->kind == DESK_CUE && ctl->state == DESK_UNKNOWN && !ctl->pending)
        centred(c, fonts->small, r.x, r.y + r.h - 18, r.w, 14, "?", DESK_MUTED);
    // The rhythm's mark beside its words, when both fit across the segment.
    int mark = icon_glyph(ctl->icon);
    int words = text_w(fonts->tile_s, ctl->label);
    if (words > r.w - 16)
        words = r.w - 16;
    int marked = mark >= 0 && words + MARK + MARK_GAP <= r.w - 16;
    int x = r.x + (r.w - words - (marked ? MARK + MARK_GAP : 0)) / 2;
    if (marked) {
        icon_paint(c, (enum icon_id)mark, x, r.y + (r.h - MARK) / 2, ink);
        x += MARK + MARK_GAP;
    }
    text_at(c, fonts->tile_s, x, r.y + (r.h - text_h(fonts->tile_s)) / 2, words, ctl->label, ink);
    pending_mark(c, ctl, r);
}

// A rig colour keeps a thin selected outline and a check badge. In hit mode
// every swatch carries a bolt, including white and pale colours.
static void paint_mini(struct canvas *c, const struct desk_control *ctl,
                       const struct desk_placement *p, enum desk_link link, int one_shot) {
    struct rect r = of(p);
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t colour = ctl->swatches ? ctl->swatch[0] : DESK_TILE;
    if (!ctl->enabled || link != DESK_LINK_READY)
        colour = desk_colour_dim(colour);
    int inset = on || ctl->pressed ? 3 : 0;
    if (inset)
        canvas_blend_round_rect(c, r.x, r.y, r.w, r.h, 10,
                                ctl->pressed ? DESK_INK : DESK_ACCENT);
    // Black and near-black need a keyline to exist on the panel.
    int brightest = (int)(colour >> 16 & 0xFF);
    if ((int)(colour >> 8 & 0xFF) > brightest)
        brightest = (int)(colour >> 8 & 0xFF);
    if ((int)(colour & 0xFF) > brightest)
        brightest = (int)(colour & 0xFF);
    uint32_t line = brightest < 0x30 ? DESK_LINE : colour;
    desk_paint_keyline(c, r.x + inset, r.y + inset, r.w - 2 * inset, r.h - 2 * inset, 8, colour, line);
    if (one_shot) {
        // The bolt on the swatch itself, centred and with no plate (owner,
        // 2026-10-28); dark wherever it contrasts more than white.
        icon_paint(c, ICON_BOLT, r.x + (r.w - MARK) / 2, r.y + (r.h - MARK) / 2,
                   desk_mark_dark(colour) ? DESK_GLASS : DESK_INK);
    }
    if (on) {
        int badge_x = r.x + 5, badge_y = r.y + r.h - 29;
        canvas_blend_round_rect(c, badge_x, badge_y, 24, 24, 8, DESK_GLASS);
        icon_paint(c, ICON_CHECK, badge_x, badge_y, DESK_INK);
    }
    pending_mark(c, ctl, r);
}

// The TEMPO SHOW panel and its heading; the numbers and the tap target are
// the speed model's, painted by the caller over this.
static void paint_tempo_frame(struct canvas *c, const struct desk_control *ctl,
                              const struct desk_placement *p, const struct desk_fonts *fonts) {
    struct rect r = of(p);
    canvas_blend_round_rect(c, r.x, r.y, r.w, r.h, DESK_PANEL_RADIUS, DESK_PANEL);
    char cap[64];
    desk_text_upper(cap, sizeof cap, ctl->label);
    text_at(c, fonts->section, r.x + DESK_PANEL_PAD, r.y + DESK_PANEL_PAD, r.w - 2 * DESK_PANEL_PAD,
            cap, DESK_MUTED);
    if (!ctl->enabled)
        centred(c, fonts->small, r.x, r.y + r.h / 2, r.w, 18, state_line(ctl, DESK_LINK_READY), DESK_MUTED);
}

static void paint_compact(struct canvas *c, const struct desk_control *ctl,
                          const struct desk_placement *p, const struct desk_fonts *fonts,
                          enum desk_link link) {
    struct rect r = of(p);
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = tile_fill(ctl);
    tile_frame(c, r, DESK_RADIUS, ctl, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = on ? DESK_ACCENT_INK : DESK_INK;
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    // The mark at the left, as on a cue.
    int mark = icon_glyph(ctl->icon);
    int tx = r.x + 8, tw = r.w - 16;
    if (mark >= 0) {
        icon_paint(c, (enum icon_id)mark, r.x + 10, r.y + (r.h - MARK) / 2, ink);
        tx = r.x + 10 + MARK + MARK_GAP;
        tw = r.x + r.w - 8 - tx;
    }
    caption(c, fonts->tile_s, tx, tw, r.y + r.h / 2, ctl->label, ink);
    pending_mark(c, ctl, r);
}

// A switch that is its mark alone, centred; lit in the accent while on.
static void paint_icon_switch(struct canvas *c, const struct desk_control *ctl,
                              const struct desk_placement *p, enum desk_link link) {
    struct rect r = of(p);
    int on = ctl->state == DESK_ON && ctl->enabled;
    uint32_t fill = tile_fill(ctl);
    // On is the highlighted square and the accent bolt, with no bar: on a
    // 28 px square the bar crossed the bolt (owner, 2026-10-29).
    if (fill == DESK_SELECTED_FILL)
        desk_paint_selected(c, r.x, r.y, r.w, r.h, DESK_RADIUS);
    else
        tile_frame(c, r, DESK_RADIUS, ctl, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = on ? DESK_ACCENT_INK : DESK_INK;
    if (!ctl->enabled || link != DESK_LINK_READY)
        ink = DESK_MUTED;
    int mark = icon_glyph(ctl->icon);
    if (mark >= 0)
        icon_paint(c, (enum icon_id)mark, r.x + (r.w - MARK) / 2, r.y + (r.h - MARK) / 2, ink);
    pending_mark(c, ctl, r);
}

// The master: a dark track, one thumb for the confirmed level, and a readout.
static void paint_master(struct canvas *c, const struct desk_control *ctl,
                         const struct desk_placement *p, const struct desk_fonts *fonts,
                         enum desk_link link) {
    struct rect r = of(p);
    desk_paint_keyline(c, r.x, r.y, r.w, r.h, DESK_RADIUS, DESK_TILE, DESK_LINE);
    int live = link == DESK_LINK_READY;
    int known = ctl->state != DESK_UNKNOWN;
    // The readout stays above the travel, never under the 24 px thumb.
    char text[16];
    if (live && known)
        snprintf(text, sizeof text, "%d%%", ctl->level * 100 / 255);
    else
        snprintf(text, sizeof text, "--");
    centred(c, fonts->section, r.x, r.y + 10, r.w, 20, "MASTER", DESK_MUTED);
    int bh = text_h(fonts->value) + 8;
    centred(c, fonts->value, r.x, r.y + 30, r.w, bh, text, DESK_INK);
    struct desk_rect track = desk_master_track(p, fonts->value);
    int track_y = track.y, track_h = track.h;
    canvas_blend_round_rect(c, track.x, track_y, track.w, track_h, 8, DESK_GLASS);
    if (known) {
        // The grip follows its own travel, matching the input inverse.
        int travel = track_h - DESK_MASTER_THUMB_H;
        int thumb_y = track_y + ((255 - ctl->level) * travel + 127) / 255;
        canvas_blend_round_rect(c, r.x + 12, thumb_y, r.w - 24, DESK_MASTER_THUMB_H, 8,
                          live ? DESK_INK : DESK_MUTED);
    }
}

// The one red on the desk: the console's own StopAll, drawn as a warning
// rather than as a cue because it has no state to light and never will.
static void paint_stop_all(struct canvas *c, const struct desk_control *ctl,
                           const struct desk_placement *p, const struct desk_fonts *fonts,
                           enum desk_link link) {
    struct rect r = of(p);
    int live = ctl->enabled && link == DESK_LINK_READY;
    uint32_t fill = live ? DESK_WARN : DESK_TILE;
    canvas_blend_round_rect(c, r.x, r.y, r.w, r.h, DESK_RADIUS, fill);
    if (ctl->pressed)
        press_outline(c, r, fill);
    uint32_t ink = live ? DESK_INK : DESK_MUTED;
    centred(c, fonts->tile, r.x, r.y + r.h / 2 - 34, r.w, 24, "PARAR", ink);
    centred(c, fonts->tile, r.x, r.y + r.h / 2 - 10, r.w, 24, "TODO", ink);
    const char *foot = live ? (ctl->detail[0] ? ctl->detail : "") : state_line(ctl, link);
    centred(c, fonts->small, r.x, r.y + r.h / 2 + 22, r.w, 18, foot, live ? DESK_WARN_INK : DESK_MUTED);
}

// ---- the bar

static const char *link_words(enum desk_link link) {
    switch (link) {
    case DESK_LINK_READY:      return "enlazado";
    case DESK_LINK_SYNCING:
    case DESK_LINK_CONNECTING: return "Esperando conexión...";
    default:                   return "Configurar master";
    }
}

static void paint_bar(struct canvas *c, const struct desk_model *model,
                      const struct desk_fonts *fonts) {
    canvas_fill_rect(c, 0, 0, DESK_W, DESK_BAR_H, DESK_GLASS);
    desk_paint_tabs(c, model, fonts);
    // The status cluster: the master's word and its dot, the fan, the
    // battery, then the two targets. The word is what matters at a glance; the address is the
    // settings' business.
    // The word: the room's state the master says runs, then the link, so
    // a family tab still says where the show is.
    char word[96];
    const char *room = NULL;
    for (int i = 0; i < model->count && !room; i++)
        if (model->control[i].kind == DESK_CUE && model->control[i].role == MAP_ROLE_STATE &&
            model->control[i].state == DESK_ON)
            room = model->control[i].label;
    if (model->link == DESK_LINK_READY && room)
        snprintf(word, sizeof word, "%s \xc2\xb7 %s", room, link_words(model->link));
    else
        snprintf(word, sizeof word, "%s", link_words(model->link));
    // Status owns only the run after the last tab, with an 8 px gutter.
    // Fit the complete room/link phrase, including future longer captions.
    int word_x = DESK_TAB_X + DESK_TABS * (DESK_TAB_W + DESK_TAB_GAP) + 4;
    int word_w = DESK_STATUS_END - word_x;
    if (word_w > 0) {
        int width = text_w(fonts->small, word);
        if (width > word_w) width = word_w;
        // The fallback has no fitted renderer: truncate before drawing too.
        if (!fonts->small) {
            size_t chars = strlen(word);
            while (chars && text_w(NULL, word) > word_w) word[--chars] = '\0';
            width = text_w(NULL, word);
        }
        text_at(c, fonts->small, DESK_STATUS_END - width, (DESK_BAR_H - text_h(fonts->small)) / 2,
                word_w, word, model->link == DESK_LINK_READY ? DESK_MUTED : DESK_WARN);
    }
    // The link at a glance: green while the master answers, red otherwise.
    canvas_blend_round_rect(c, DESK_LINK_DOT_X, (DESK_BAR_H - DESK_LINK_DOT) / 2, DESK_LINK_DOT,
                      DESK_LINK_DOT, DESK_LINK_DOT / 2,
                      model->link == DESK_LINK_READY ? DESK_OK : DESK_WARN);
    icon_paint_wifi(c, 776, 12, model->wifi_bars, DESK_INK, DESK_LINE);
    icon_paint_battery(c, 816, 12, model->battery < 0 ? 0 : model->battery, model->charging, DESK_INK,
                       model->battery >= 0 && model->battery <= 20 ? DESK_WARN : DESK_INK, DESK_GLASS);
    char pct[16];
    if (model->battery >= 0)
        snprintf(pct, sizeof pct, "%d%%", model->battery);
    else
        snprintf(pct, sizeof pct, "--");
    text_at(c, fonts->small, 848, (DESK_BAR_H - text_h(fonts->small)) / 2, 60, pct, DESK_MUTED);
    struct desk_rect g = desk_view_gear(), l = desk_view_lock_target();
    if (model->setup_open)
        canvas_blend_round_rect(c, g.x + 4, g.y + 4, g.w - 8, g.h - 8, 10, DESK_RAISED);
    icon_paint(c, ICON_GEAR, g.x + 12, g.y + 12, model->setup_open ? DESK_INK : DESK_MUTED);
    if (model->locked)
        canvas_blend_round_rect(c, l.x + 4, l.y + 4, l.w - 8, l.h - 8, 10, DESK_INK);
    icon_paint(c, model->locked ? ICON_LOCK : ICON_LOCK_OPEN, l.x + 12, l.y + 12,
               model->locked ? DESK_GLASS : DESK_MUTED);
}

static void paint_headings(struct canvas *c, const struct desk_model *model,
                           const struct desk_fonts *fonts) {
    for (int i = 0; i < model->layout.headings; i++) {
        const struct desk_heading *h = &model->layout.heading[i];
        if (h->page != model->page || h->bank != model->bank)
            continue;
        // A section of hits held on the Mac says so in its title, once.
        int held_only = 1, any = 0;
        for (int j = 0; j < model->count; j++) {
            const struct desk_control *ctl = &model->control[j];
            if (ctl->page != model->page || ctl->section != h->section)
                continue;
            any = 1;
            if (ctl->enabled || strncmp(ctl->reason, "held", 4) != 0)
                held_only = 0;
        }
        char text[192];
        const char *elsewhere = NULL;
        if (h->parts > 1)
            for (int j = 0; j < model->layout.placements && !elsewhere; j++) {
                const struct desk_placement *p = &model->layout.placement[j];
                if (p->page != model->page || p->bank == model->bank || p->tile == TILE_MASTER ||
                    p->tile == TILE_PANIC)
                    continue;
                const struct desk_control *ctl = &model->control[p->control];
                if (ctl->section == h->section && ctl->state == DESK_ON)
                    elsewhere = ctl->label;
            }
        const char *title = desk_panel_title(model, h);
        if (h->parts > 1 && elsewhere)
            snprintf(text, sizeof text, "%.60s  %d/%d  \xc2\xb7  %.47s", title, h->part, h->parts, elsewhere);
        else if (h->parts > 1)
            snprintf(text, sizeof text, "%.60s  %d/%d", title, h->part, h->parts);
        else
            snprintf(text, sizeof text, "%.60s%s", title, any && held_only ? "  \xc2\xb7  solo en el Mac" : "");
        // A panel whose tiles carry no words names the one running.
        const struct desk_control *running =
            h->kind == DESK_PANEL_NAMES_RUNNING ? desk_panel_running(model, h) : NULL;
        if (running) {
            char named[192];
            snprintf(named, sizeof named, "%.60s \xc2\xb7 %.60s", text, desk_rig_name(running->label));
            snprintf(text, sizeof text, "%s", named);
        }
        if (h->h <= 0) {
            int title_w = h->kind == DESK_PANEL_NAMES_RUNNING ? h->w - 2 * DESK_PANEL_PAD - 28 - DESK_GAP : h->w;
            desk_paint_section(c, fonts, h->x, h->y, title_w, text);
            continue;
        }
        // The hero paints its own panel and heading.
        if (h->kind == DESK_PANEL_ROOM) {
            desk_paint_room(c, model, h, fonts);
            continue;
        }
        // A flowed section's panel, its heading inside in capitals.
        canvas_blend_round_rect(c, h->x, h->y, h->w, h->h, DESK_PANEL_RADIUS, DESK_PANEL);
        char cap[192];
        desk_text_upper(cap, sizeof cap, text);
        text_at(c, fonts->section, h->x + DESK_PANEL_PAD, h->y + DESK_PANEL_PAD,
                h->w - 2 * DESK_PANEL_PAD, cap, DESK_MUTED);
    }
}

// Section names and parts describe each bank; the small number keeps its place
// in the sequence. The current pill is ink with dark words, the others tiles.
static void paint_pager(struct canvas *c, const struct desk_model *model,
                        const struct desk_fonts *fonts) {
    int banks = model->layout.pages > 0 ? model->layout.banks[model->page] : 1;
    if (banks <= 1)
        return;
    for (int i = 0; i < banks; i++) {
        struct desk_rect b = desk_view_pager(i, &model->layout, model->page);
        int current = i == model->bank;
        desk_paint_keyline(c, b.x, b.y, b.w, b.h, DESK_RADIUS, current ? DESK_INK : DESK_TILE,
                           current ? DESK_INK : DESK_LINE);
        char digit[16];
        snprintf(digit, sizeof digit, "%d", i + 1);
        char label[DESK_PAGER_BANK_CAPTION_MAX];
        desk_pager_bank_caption(model, fonts->tab, model->page, i, b.w - 64, label);
        uint32_t ink = current ? DESK_GLASS : DESK_MUTED;
        if (!label[0]) {
            centred(c, fonts->tab, b.x, b.y, b.w, b.h, digit, ink);
        } else {
            text_at(c, fonts->small, b.x + 16, b.y + (b.h - text_h(fonts->small)) / 2,
                    24, digit, ink);
            text_at(c, fonts->tab, b.x + 40, b.y + (b.h - text_h(fonts->tab)) / 2,
                    b.w - 64, label, ink);
        }
        int running = 0;
        for (int j = 0; j < model->layout.placements && !running && i != model->bank; j++) {
            const struct desk_placement *p = &model->layout.placement[j];
            if (p->page == model->page && p->bank == i && p->tile != TILE_MASTER &&
                p->tile != TILE_PANIC && model->control[p->control].state == DESK_ON)
                running = 1;
        }
        if (running)
            canvas_blend_round_rect(c, b.x + b.w - 12, b.y + 4, 8, 8, 4, DESK_ACCENT);
    }
}

static void paint_mismatch_banner(struct canvas *c, const struct desk_model *model,
                                  const struct desk_fonts *fonts) {
    if (model->link != DESK_LINK_READY || !model->mismatch)
        return;
    int x = 248, y = 238, w = 528, h = 116;
    canvas_blend_round_rect(c, DESK_CONTENT_X, DESK_CONTENT_Y,
                            DESK_W - 2 * DESK_CONTENT_X, DESK_CONTENT_END - DESK_CONTENT_Y,
                            16, 0xD9000000u);
    canvas_blend_round_rect(c, x, y, w, h, 14, DESK_TILE);
    canvas_fill_rect(c, x + 16, y + 17, 4, h - 34, DESK_WARN);
    centred(c, fonts->tile, x + 24, y + 25, w - 48, 32, "Otro show cargado en el Mac", DESK_INK);
    centred(c, fonts->label, x + 24, y + 66, w - 48, 24, "Controles desactivados", DESK_MUTED);
}

void desk_paint_overlays(struct canvas *canvas, const struct desk_model *model,
                         const struct desk_fonts *fonts) {
    paint_mismatch_banner(canvas, model, fonts);
    if (model->locked) {
        int x = DESK_CONTENT_X, w = DESK_CONTENT_W;
        canvas_blend_round_rect(canvas, x, DESK_H - 56, w, 40, 10, DESK_INK);
        centred(canvas, fonts->label, x, DESK_H - 56, w, 40,
                "Bloqueado - mantén el candado para desbloquear", DESK_GLASS);
    }
}

void desk_paint(struct canvas *canvas, const struct desk_model *model,
                const struct desk_fonts *fonts) {
    canvas_fill_rect(canvas, 0, 0, DESK_W, DESK_H, DESK_GLASS);
    paint_bar(canvas, model, fonts);
    if (model->layout.pages > 0 && model->layout.tab[model->page] == DESK_TAB_EFECTOS)
        desk_paint_families(canvas, model, fonts);
    paint_headings(canvas, model, fonts);
    // OFF is chosen only on the master's word: linked, and every rhythm known off.
    int none_running = model->link == DESK_LINK_READY;
    for (int i = 0; i < model->count; i++)
        if (model->control[i].kind == DESK_CUE && model->control[i].role == MAP_ROLE_HAZE &&
            (model->control[i].state != DESK_OFF || model->control[i].pending))
            none_running = 0;
    for (int i = 0; i < model->layout.placements; i++) {
        const struct desk_placement *p = &model->layout.placement[i];
        if (p->page != model->page || p->bank != model->bank)
            continue;
        if (p->control < 0 || p->control >= model->count)
            continue;
        const struct desk_control *ctl = &model->control[p->control];
        switch (p->tile) {
        case TILE_MASTER:  paint_master(canvas, ctl, p, fonts, model->link); break;
        case TILE_PANIC:   paint_stop_all(canvas, ctl, p, fonts, model->link); break;
        case TILE_SWATCH:  paint_swatch(canvas, ctl, p, fonts, model->link); break;
        case TILE_NUMBER:  desk_paint_number(canvas, ctl, p, fonts, model->link); break;
        case TILE_TRAJECTORY: desk_paint_trajectory(canvas, ctl, p, fonts, model->link); break;
        case TILE_HOLD:
        case TILE_FOG:     desk_paint_hold(canvas, ctl, p, fonts, model->link); break;
        case TILE_STATE:   paint_state(canvas, ctl, p, fonts, model->link); break;
        case TILE_SEGMENT: paint_segment(canvas, ctl, p, fonts, model->link, none_running); break;
        case TILE_MINI:    paint_mini(canvas, ctl, p, model->link, model->color_one_shot); break;
        case TILE_TEMPO:   paint_tempo_frame(canvas, ctl, p, fonts); break;
        case TILE_COMPACT: paint_compact(canvas, ctl, p, fonts, model->link); break;
        case TILE_ICON:    paint_icon_switch(canvas, ctl, p, model->link); break;
        default:           paint_cue(canvas, ctl, p, fonts, model->link); break;
        }
    }
    paint_pager(canvas, model, fonts);
}
