// SOURCES: desk_model.c desk_master_level_at.c desk_paint.c desk_master_track.c desk_pager_caption.c desk_pager_bank_caption.c icon.c icon_glyph.c desk_caption.c desk_view.c desk_view_pager.c desk_pager_label.c canvas.c canvas_blend.c font.c desk_fonts.c desk_paint_keyline.c desk_paint_text_centred.c desk_paint_tabs.c desk_paint_families.c desk_tab_page.c desk_tab_title.c desk_view_family.c desk_text_upper.c desk_colour_dim.c desk_panel_holds.c desk_panel_running.c desk_rig_name.c desk_paint_room.c desk_paint_room_status.c desk_room_summary.c desk_paint_text_at.c desk_paint_number.c desk_paint_trajectory.c desk_paint_tile_base.c icon_paint_large.c desk_caption_number.c desk_trajectory_of.c desk_damage_running.c desk_paint_hold.c desk_paint_selected.c desk_paint_selected_mark.c icon_paint_mid.c desk_paint_room_picture.c icon_paint_hero.c desk_mark_dark.c desk_luminance.c desk_srgb_linear.c desk_panel_title.c
// The master is unknown until the master speaks, a drag moves only what the
// desk asked for, and a finger on a tile never repaints what the show said.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "canvas.h"
#include "desk_layout.h"
#include "desk_master_metrics.h"
#include "desk_model.h"
#include "desk_master_track.h"
#include "desk_paint.h"

static int count(const struct canvas *c, int x, int y, int w, int h, uint32_t col) {
    int n = 0;
    for (int j = y; j < y + h; j++)
        for (int i = x; i < x + w; i++)
            if (c->px[j * c->w + i] == col)
                n++;
    return n;
}

int main(void) {
    struct desk_model m;
    desk_init(&m);
    struct desk_control master = { .kind = DESK_MASTER, .enabled = 1, .widget_id = -1,
        .function_id = -1 };
    strcpy(master.label, "Master");
    struct desk_control cue = { .kind = DESK_CUE, .enabled = 1, .widget_id = 4,
        .function_id = 720 };
    strcpy(cue.label, "AUTO");
    int im = desk_add(&m, &master);
    int ic = desk_add(&m, &cue);
    // A one-page layout by hand: the master in its column, the cue on the grid.
    struct desk_layout layout;
    memset(&layout, 0, sizeof layout);
    layout.pages = 1;
    layout.banks[0] = 1;
    layout.placement[0] = (struct desk_placement){ im, 0, 0, DESK_MASTER_TILE_X, DESK_MASTER_TILE_Y,
                                                   DESK_MASTER_TILE_W, DESK_MASTER_TILE_H, TILE_MASTER };
    layout.placement[1] = (struct desk_placement){ ic, 0, 0, DESK_GRID_X, DESK_GRID_Y,
                                                   DESK_TILE_W, DESK_TILE_H, TILE_CUE };
    layout.placements = 2;
    desk_set_layout(&m, &layout);
    desk_set_link(&m, DESK_LINK_READY);
    const struct desk_placement *mp = &layout.placement[0], *cp = &layout.placement[1];
    assert(m.control[im].state == DESK_UNKNOWN);

    struct canvas c = { .px = calloc(DESK_W * DESK_H, 4), .w = DESK_W, .h = DESK_H };
    struct desk_fonts fonts;
    memset(&fonts, 0, sizeof fonts);
    struct desk_rect track = desk_master_track(mp, fonts.value);
    desk_paint(&c, &m, &fonts);
    // Unknown: no fill and no thumb in the master tile; the accent is never a level.
    assert(count(&c, mp->x, mp->y, mp->w, mp->h, DESK_RAISED) == 0);
    assert(count(&c, mp->x, mp->y, mp->w, mp->h, DESK_ACCENT) == 0);

    // The first push, whatever its value, makes the level known.
    desk_apply_master(&m, 0);
    assert(m.control[im].state == DESK_ON && m.control[im].level == 0);
    desk_apply_master(&m, 128);
    assert(m.control[im].level == 128 && m.control[im].requested_level == 128);
    desk_paint(&c, &m, &fonts);
    int half = count(&c, mp->x, mp->y, mp->w, mp->h, DESK_INK);
    assert(half > mp->w * DESK_MASTER_THUMB_H / 2);
    assert(count(&c, mp->x, mp->y, mp->w, mp->h, DESK_RAISED) == 0);

    // A drag moves the requested level and the action, never the confirmed one.
    struct desk_action a = desk_touch_down(&m, 0, mp->x + 10, mp->y + mp->h - 1, &track);
    assert(a.kind == DESK_ACT_MASTER && a.value == 0);
    assert(m.control[im].requested_level == 0 && m.control[im].level == 128);
    a = desk_touch_move(&m, 0, mp->x + 10, mp->y, &track);
    assert(a.kind == DESK_ACT_MASTER && a.value == 255 && m.control[im].level == 128);
    desk_paint(&c, &m, &fonts);
    int during = count(&c, mp->x, mp->y, mp->w, mp->h, DESK_INK);
    assert(during <= half && during > half * 9 / 10);    // the confirmed thumb did not move
    // A push while the finger is down does not yank the request.
    desk_apply_master(&m, 140);
    assert(m.control[im].level == 140 && m.control[im].requested_level == 255);
    desk_touch_up(&m, 0, mp->x + 10, mp->y);
    // The master's own push, arriving after, is what moves it.
    desk_apply_master(&m, 255);
    assert(m.control[im].requested_level == 255);
    desk_paint(&c, &m, &fonts);
    assert(count(&c, mp->x, mp->y, mp->w, mp->h, DESK_RAISED) == 0);

    // A pressed cue keeps its fill: a running cue keeps the selected fill
    // under the finger, and the outline is ink.
    desk_apply_function(&m, 720, 1);
    desk_paint(&c, &m, &fonts);
    int lit = count(&c, cp->x, cp->y, cp->w, cp->h, DESK_SELECTED_FILL);
    assert(lit > cp->w * cp->h / 2);
    desk_touch_down(&m, 1, cp->x + 20, cp->y + 20, NULL);
    desk_paint(&c, &m, &fonts);
    int lit_pressed = count(&c, cp->x, cp->y, cp->w, cp->h, DESK_SELECTED_FILL);
    assert(lit_pressed > lit * 8 / 10);
    assert(count(&c, cp->x, cp->y, cp->w, cp->h, DESK_INK) > 2 * (cp->w + cp->h));
    free(c.px);
    printf("desk_master ok\n");
    return 0;
}
