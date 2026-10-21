// SOURCES: desk_model.c desk_master_level_at.c desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c vcjson.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_panel_holds.c
// What changed is what gets painted: a push lights one tile's rectangle, a
// view change everything, and a clip keeps a primitive inside its damage.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_model.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *b = malloc((size_t)n + 1);
    assert(fread(b, 1, (size_t)n, f) == (size_t)n);
    b[n] = 0;
    fclose(f);
    *len = (size_t)n;
    return b;
}

int main(void) {
    size_t n;
    char *vc = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &n);
    struct vc_doc doc;
    assert(vc_parse(vc, n, &doc) == 0);
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    int auto_function = -1, charla_function = -1;
    for (int i = 0; i < map.count; i++) {
        if (strcmp(map.control[i].key, "auto") == 0)
            auto_function = map.control[i].function_id;
        if (strcmp(map.control[i].key, "charla") == 0)
            charla_function = map.control[i].function_id;
    }
    assert(auto_function >= 0 && charla_function >= 0);
    struct desk_model m;
    showmap_build(&m, &map, &doc);
    struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    desk_set_layout(&m, &layout);
    desk_set_link(&m, DESK_LINK_READY);

    int x, y, w, h;
    // After setup everything is damaged, once.
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1 && w < 0);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 0);

    // A push lights AUTO: its rectangle on LIVE, the hero that names it and
    // the bar's word, and nothing below the compact hero.
    desk_apply_function(&m, auto_function, 1);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    int auto_ix = -1;
    for (int i = 0; i < m.count; i++)
        if (strcmp(m.control[i].label, "AUTO") == 0)
            auto_ix = i;
    const struct desk_placement *ap = desk_placement_of(&m, auto_ix);
    assert(ap && w > 0 && x <= ap->x && y == 0 && x + w >= ap->x + ap->w &&
           y + h == DESK_HERO_Y + DESK_HERO_H);
    assert(x <= DESK_HERO_X && x + w >= DESK_HERO_X + DESK_HERO_W && x + w <= DESK_STATUS_END);

    // 2026-10-21: the tablet left OFF selected beside a running haze rhythm.
    // Its selection is derived from every rhythm, so either edge must repaint it.
    int haze_function = -1, off_ix = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "cada-1-min") == 0)
            haze_function = map.control[i].function_id;
    for (int i = 0; i < m.count; i++)
        if (m.control[i].kind == DESK_HAZE_OFF)
            off_ix = i;
    assert(haze_function >= 0 && off_ix >= 0);
    const struct desk_placement *off = desk_placement_of(&m, off_ix);
    assert(off);
    desk_apply_function(&m, haze_function, 1);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    assert(x <= off->x && y <= off->y && x + w >= off->x + off->w && y + h >= off->y + off->h);
    desk_touch_down(&m, 0, off->x + off->w / 2, off->y + off->h / 2, NULL);
    struct desk_action off_action = desk_touch_up(&m, 0, off->x + off->w / 2,
                                                  off->y + off->h / 2);
    int haze_widget = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "cada-1-min") == 0)
            haze_widget = map.control[i].widget_id;
    assert(off_action.kind == DESK_ACT_HAZE_OFF && off_action.widget_id == haze_function);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    desk_apply_function(&m, haze_function, 0);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    assert(x <= off->x && y <= off->y && x + w >= off->x + off->w && y + h >= off->y + off->h);
    // 2026-10-21: tapping OFF before QLC+ reports the newly selected rhythm
    // must still stop it. A widget toggle here used to send nothing.
    int haze_ix = -1;
    for (int i = 0; i < m.count; i++)
        if (m.control[i].widget_id == haze_widget)
            haze_ix = i;
    assert(haze_ix >= 0);
    const struct desk_placement *haze = desk_placement_of(&m, haze_ix);
    assert(haze);
    desk_touch_down(&m, 0, haze->x + 2, haze->y + 2, NULL);
    struct desk_action start_haze = desk_touch_up(&m, 0, haze->x + 2, haze->y + 2);
    assert(start_haze.kind == DESK_ACT_TOGGLE && start_haze.widget_id == haze_widget);
    desk_note_sent(&m, haze_widget, 1000);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    assert(x <= off->x && y <= off->y && x + w >= off->x + off->w && y + h >= off->y + off->h);
    desk_touch_down(&m, 0, off->x + 2, off->y + 2, NULL);
    off_action = desk_touch_up(&m, 0, off->x + 2, off->y + 2);
    assert(off_action.kind == DESK_ACT_HAZE_OFF && off_action.widget_id == haze_function);

    // 2026-10-21: a colour tile can send its fixed look or the matching
    // bounded hit, while changing the local mode sends nothing to QLC+.
    int rig_ix = -1, mode_ix = -1, hit_widget = -1;
    for (int i = 0; i < map.count; i++) {
        if (strcmp(map.control[i].key, "rig-rosa") == 0)
            rig_ix = i;
        if (strcmp(map.control[i].key, "rosa") == 0)
            hit_widget = map.control[i].widget_id;
    }
    for (int i = 0; i < m.count; i++)
        if (m.control[i].kind == DESK_COLOR_MODE)
            mode_ix = i;
    assert(rig_ix >= 0 && mode_ix >= 0 && hit_widget >= 0);
    assert(m.control[mode_ix].enabled && m.control[rig_ix].shot_widget_id == hit_widget);
    const struct desk_placement *rig = desk_placement_of(&m, rig_ix);
    const struct desk_placement *mode = desk_placement_of(&m, mode_ix);
    assert(rig && mode);
    struct desk_action action = desk_touch_down(&m, 0, rig->x + 2, rig->y + 2, NULL);
    assert(action.kind == DESK_ACT_NONE);
    action = desk_touch_up(&m, 0, rig->x + 2, rig->y + 2);
    assert(action.kind == DESK_ACT_TOGGLE && action.widget_id == m.control[rig_ix].widget_id);
    desk_touch_down(&m, 0, mode->x + 2, mode->y + 2, NULL);
    action = desk_touch_up(&m, 0, mode->x + 2, mode->y + 2);
    assert(action.kind == DESK_ACT_NONE && m.color_one_shot && m.control[mode_ix].state == DESK_ON);
    desk_touch_down(&m, 0, rig->x + 2, rig->y + 2, NULL);
    action = desk_touch_up(&m, 0, rig->x + 2, rig->y + 2);
    assert(action.kind == DESK_ACT_TOGGLE && action.widget_id == hit_widget);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);

    int white_hit = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "blanco") == 0)
            white_hit = map.control[i].widget_id;
    int white_ix = DESK_WHITE_INDEX(&map);
    const struct desk_placement *white = desk_placement_of(&m, white_ix);
    assert(white && white->tile == TILE_MINI && m.control[white_ix].shot_widget_id == white_hit);
    desk_touch_down(&m, 0, white->x + 2, white->y + 2, NULL);
    action = desk_touch_up(&m, 0, white->x + 2, white->y + 2);
    assert(action.kind == DESK_ACT_TOGGLE && action.widget_id == white_hit);
    desk_touch_down(&m, 0, mode->x + 2, mode->y + 2, NULL);
    desk_touch_up(&m, 0, mode->x + 2, mode->y + 2);
    desk_touch_down(&m, 0, white->x + 2, white->y + 2, NULL);
    action = desk_touch_up(&m, 0, white->x + 2, white->y + 2);
    assert(action.kind == DESK_ACT_TOGGLE && action.widget_id == m.control[white_ix].widget_id);
    desk_take_damage(&m, &x, &y, &w, &h);

    // A push for a control on another page damages nothing on this view.
    int gobo_fn = -1;
    for (int i = 0; i < m.count; i++)
        if (strcmp(m.control[i].label, "Gobo 1") == 0)
            gobo_fn = m.control[i].function_id;
    assert(gobo_fn >= 0);
    desk_apply_function(&m, gobo_fn, 1);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 0);

    // Two changes: the union, the same band.
    desk_apply_function(&m, auto_function, 0);
    desk_apply_function(&m, charla_function, 1);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1);
    assert(x <= ap->x && y == 0 && w > ap->w && y + h == DESK_HERO_Y + DESK_HERO_H);

    // A view change is everything; a status rectangle is itself.
    desk_set_view(&m, 1, 0);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1 && w < 0);
    desk_damage_rect(&m, 0, 0, 1024, 48);
    assert(desk_take_damage(&m, &x, &y, &w, &h) == 1 && x == 0 && y == 0 && w == 1024 && h == 48);

    vc_free(&doc);
    free(vc);
    printf("desk_damage ok\n");
    return 0;
}
