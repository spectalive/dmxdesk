#include "desk_speed_paint.h"

#include <stdio.h>
#include <string.h>

#include "canvas_blend.h"
#include "desk_layout.h"
#include "desk_paint_keyline.h"
#include "desk_speed_layout.h"
#include "desk_speed_target_rect.h"
#include "desk_text_upper.h"
#include "speed_factor.h"

static int text_h(struct font *f) { return f ? font_height(f) : 15; }

static void text_at(struct canvas *c, struct font *f, int x, int y, int max_w, const char *s, uint32_t col) {
    if (f)
        font_draw_fit(f, c, x, y + font_baseline(f), max_w, s, col);
    else
        canvas_text(c, x, y, s, 3, col);
}

static void centred(struct canvas *c, struct font *f, int x, int y, int w, int h, const char *s, uint32_t col) {
    int width = f ? font_width(f, s) : canvas_text_width(s, 3);
    if (width > w)
        width = w;
    text_at(c, f, x + (w - width) / 2, y + (h - text_h(f)) / 2, w, s, col);
}

// A target: a tile in the desk's palette inside its keyline, the ground
// under it showing and a dimmer keyline when dead, an ink outline while a
// finger is on it. The accent is never a target's: it is
// the master's word, and a tempo has no "on".
static void target(struct canvas *c, struct font *f, int x, int y, int w, int h,
                   const char *label, int enabled, int pressed, uint32_t fill, uint32_t dead) {
    if (!enabled)
        fill = dead;
    desk_paint_keyline(c, x, y, w, h, 12, fill, enabled ? DESK_LINE : DESK_RAISED);
    if (pressed && enabled) {
        canvas_blend_round_rect(c, x, y, w, h, 12, DESK_INK);
        canvas_blend_round_rect(c, x + 3, y + 3, w - 6, h - 6, 9, fill);
    }
    centred(c, f, x, y, w, h, label, enabled ? DESK_INK : DESK_MUTED);
}

// The words under a dial's number: the note that matters most, then the
// change on its way, then what the number is, or why there is none.
static const char *under_number(const struct desk_dial *d) {
    if (d->note[0])
        return d->note;
    if (d->pending)
        return "enviado";
    if (d->known && d->base_ms > 0)
        return "BPM base";
    return d->enabled && d->known ? "No base time" : d->reason;
}

static void card(struct canvas *c, const struct desk_speed *s, int i, const struct desk_fonts *fonts) {
    const struct desk_dial *d = &s->dial[i];
    int x = SPEED_CARD_X(i), y = SPEED_CARD_Y;
    canvas_blend_round_rect(c, x, y, SPEED_CARD_W, SPEED_CARD_H, SPEED_RADIUS, DESK_PANEL);
    // The caption in capitals, as the dial names itself.
    char cap[MAP_CAPTION_MAX];
    desk_text_upper(cap, sizeof cap, d->caption);
    text_at(c, fonts->tab, x + SPEED_PAD, y + SPEED_PAD, SPEED_CARD_W - 2 * SPEED_PAD, cap, DESK_MUTED);
    // The number large, its meaning under it, centred left of the multiplier.
    char big[16];
    int live = d->enabled && d->known && d->base_ms > 0;
    if (live)
        snprintf(big, sizeof big, "%d", desk_speed_bpm(d));
    else
        snprintf(big, sizeof big, "--");
    struct desk_rect factor = desk_speed_target_rect(i, SPEED_T_FACTOR_ONE);
    int rx = x + SPEED_PAD, rw = factor.x - 8 - rx;
    int top = y + 36;
    centred(c, fonts->huge, rx, top, rw, text_h(fonts->huge), big, live ? DESK_INK : DESK_MUTED);
    centred(c, fonts->label, rx, top + text_h(fonts->huge), rw, text_h(fonts->label) + 4,
            under_number(d), d->note[0] ? DESK_INK : DESK_MUTED);

    int pressed_here = s->capture_dial == i;
    // The multiplier reads the dial's factor and, away from x1, takes it back.
    char times[32];
    if (live && speed_factor_valid(d->factor) && d->factor >= SPEED_FACTOR_MIN)
        snprintf(times, sizeof times, "Tiempo x%s", speed_factor_name(d->factor));
    else
        snprintf(times, sizeof times, "Tiempo --");
    static const struct { enum speed_target t; const char *label; } cells[] = {
        { SPEED_T_TAP, "Tap" }, { SPEED_T_FACTOR_ONE, NULL },
        { SPEED_T_BPM_DOWN, "-1 BPM" }, { SPEED_T_BPM_UP, "+1 BPM" },
        { SPEED_T_HALF, "M\xc3\xa1s lento x2" }, { SPEED_T_DOUBLE, "M\xc3\xa1s r\xc3\xa1pido x2" },
    };
    // Live targets are tiles on the card's panel; dead ones show the panel.
    for (size_t k = 0; k < sizeof cells / sizeof cells[0]; k++) {
        struct desk_rect r = desk_speed_target_rect(i, cells[k].t);
        target(c, cells[k].t == SPEED_T_FACTOR_ONE ? fonts->label : fonts->tile, r.x, r.y, r.w, r.h,
               cells[k].label ? cells[k].label : times, desk_speed_target_enabled(s, i, cells[k].t),
               pressed_here && s->capture == cells[k].t, DESK_TILE, DESK_PANEL);
    }
}

// TEMPO SHOW on DIRECTO: a 88 px TAP at the panel's right under its
// heading, a rule between it and the number.
void desk_speed_tempo_tap_rect(int x, int y, int w, int h, int *tx, int *ty, int *tw, int *th) {
    *tw = 88;
    *th = h - 48;
    *tx = x + w - 12 - *tw;
    *ty = y + 24;
}

void desk_speed_paint_tempo(struct canvas *c, const struct desk_speed *s, int i,
                            const struct desk_fonts *fonts, int x, int y, int w, int h, int pressed) {
    if (i < 0 || i >= s->dials)
        return;
    const struct desk_dial *d = &s->dial[i];
    char big[16];
    int live = d->enabled && d->known && d->base_ms > 0;
    if (live)
        snprintf(big, sizeof big, "%d", desk_speed_bpm(d));
    else
        snprintf(big, sizeof big, "--");
    int tx, ty, tw, th;
    desk_speed_tempo_tap_rect(x, y, w, h, &tx, &ty, &tw, &th);
    // The number under the panel's heading, then one line: base BPM, or
    // the note that matters more.
    int nw = tx - 24 - (x + 12);
    text_at(c, fonts->big, x + 12, y + 34, nw, big, live ? DESK_INK : DESK_MUTED);
    text_at(c, fonts->small, x + 12, y + h - 12 - text_h(fonts->small), nw, under_number(d), DESK_MUTED);
    canvas_fill_rect(c, tx - 12, ty, 1, th, DESK_LINE);
    int enabled = desk_speed_target_enabled(s, i, SPEED_T_TAP);
    target(c, fonts->tile, tx, ty, tw, th, "TAP", enabled, pressed, DESK_RAISED, DESK_PANEL);
}

void desk_speed_paint(struct canvas *c, const struct desk_speed *s, const struct desk_fonts *fonts) {
    for (int i = 0; i < s->dials && i < 2; i++)
        card(c, s, i, fonts);
    if (s->dials >= 2) {
        struct desk_rect r = desk_speed_target_rect(-1, SPEED_T_BOTH);
        int enabled = desk_speed_target_enabled(s, 0, SPEED_T_BOTH);
        target(c, fonts->tile, r.x, r.y, r.w, r.h, "TAP ambos", enabled, s->capture == SPEED_T_BOTH,
               DESK_TILE, DESK_GLASS);
    }
}
