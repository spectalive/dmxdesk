// SOURCES: desk_speed.c desk_speed_paint.c desk_speed_tempo_step_rect.c desk_tap.c speed_factor.c desk_model.c desk_master_level_at.c desk_paint.c desk_master_track.c desk_pager_caption.c desk_pager_bank_caption.c icon.c icon_glyph.c desk_caption.c desk_view.c desk_view_pager.c desk_pager_label.c desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c showmap_check_priority.c vcjson.c canvas.c canvas_blend.c font.c desk_fonts.c desk_paint_keyline.c desk_paint_text_centred.c desk_paint_tabs.c desk_paint_families.c desk_tab_page.c desk_tab_title.c desk_view_family.c desk_family_icon.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_text_upper.c desk_colour_dim.c desk_panel_holds.c desk_panel_running.c desk_rig_name.c desk_paint_room.c desk_paint_room_status.c desk_room_summary.c desk_paint_text_at.c desk_speed_target_rect.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_paint_number.c desk_paint_trajectory.c desk_paint_tile_base.c icon_paint_large.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_paint_hold.c desk_paint_selected.c desk_paint_selected_mark.c icon_paint_mid.c desk_paint_room_picture.c icon_paint_hero.c desk_surface_find.c desk_surface_free.c desk_surface_is_button.c qlc_surface_kind.c qlc_surface_of.c
// The desk, built from the generated Vibra map against the real console
// document, pressed once, and painted. Writes $TEST_OUT/desk.ppm so the screen can be
// looked at on a laptop before it reaches the tablet, and one render per tab:
// every EFECTOS family and bank, and RITMO with its speed cards.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "canvas.h"
#include "canvas_blend.h"
#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_model.h"
#include "desk_paint.h"
#include "desk_speed.h"
#include "desk_speed_paint.h"
#include "desk_tab.h"
#include "desk_tab_page.h"
#include "desk_view.h"
#include "desk_view_family.h"
#include "icon_data.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"
#include "test_surface.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    assert(fseek(f, 0, SEEK_END) == 0);
    long size = ftell(f);
    assert(size > 0);
    assert(fseek(f, 0, SEEK_SET) == 0);
    char *buf = malloc((size_t)size + 1);
    assert(buf);
    assert(fread(buf, 1, (size_t)size, f) == (size_t)size);
    fclose(f);
    buf[size] = '\0';
    *len = (size_t)size;
    return buf;
}

static const struct desk_control *by_label(const struct desk_model *m,
                                           const char *label) {
    for (int i = 0; i < m->count; i++) {
        if (strcmp(m->control[i].label, label) == 0)
            return &m->control[i];
    }
    return NULL;
}

static void write_ppm(const struct canvas *c, const char *path) {
    FILE *f = fopen(path, "wb");
    assert(f);
    fprintf(f, "P6\n%d %d\n255\n", c->w, c->h);
    for (int i = 0; i < c->w * c->h; i++) {
        uint32_t p = c->px[i];
        unsigned char rgb[3] = { (p >> 16) & 0xff, (p >> 8) & 0xff, p & 0xff };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

int main(void) {
    size_t len;
    char *json = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &len);
    struct vc_doc console;
    assert(vc_parse(json, len, &console) == 0);
    free(json);

    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    assert(strcmp(map.key, "vibra") == 0);

    int auto_function = -1, charla_function = -1;
    for (int i = 0; i < map.count; i++) {
        if (strcmp(map.control[i].key, "auto") == 0)
            auto_function = map.control[i].function_id;
        if (strcmp(map.control[i].key, "charla") == 0)
            charla_function = map.control[i].function_id;
    }
    assert(auto_function >= 0 && charla_function >= 0);
    struct desk_model model;
    int enabled = showmap_build(&model, &map, test_surface(&console));
    assert(enabled > 100);
    struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    desk_set_layout(&model, &layout);

    // Every placement lands inside the chrome, and cues never under the master.
    for (int i = 0; i < layout.placements; i++) {
        const struct desk_placement *p = &layout.placement[i];
        assert(p->x >= DESK_CONTENT_X && p->y >= DESK_BAR_H);
        assert(p->x + p->w <= DESK_W && p->y + p->h <= DESK_H);
        if (p->tile != TILE_MASTER && p->tile != TILE_PANIC)
            assert(p->x + p->w <= DESK_MASTER_X);
    }

    // Every enabled tile starts from the console document's own state rather
    // than from nothing: in this fixture nothing was running.
    assert(by_label(&model, "AUTO")->state == DESK_OFF);

    // Nothing is pressable while the link is down, however good the map is.
    const struct desk_control *auto_ctl = by_label(&model, "AUTO");
    const struct desk_placement *ap = desk_placement_of(&model, (int)(auto_ctl - model.control));
    int cx = ap->x + ap->w / 2, cy = ap->y + ap->h / 2;
    assert(desk_touch_down(&model, 0, cx, cy, NULL).kind == DESK_ACT_NONE);
    assert(desk_touch_up(&model, 0, cx, cy).kind == DESK_ACT_NONE);

    desk_set_link(&model, DESK_LINK_READY);
    assert(desk_touch_down(&model, 0, cx, cy, NULL).kind == DESK_ACT_NONE);
    assert(by_label(&model, "AUTO")->pressed == 1);
    // A second finger cannot fire another tile while the first is captured.
    const struct desk_control *charla = by_label(&model, "CHARLA");
    const struct desk_placement *chp = desk_placement_of(&model, (int)(charla - model.control));
    assert(desk_touch_down(&model, 1, chp->x + 10, chp->y + 10, NULL).kind == DESK_ACT_NONE);
    assert(by_label(&model, "CHARLA")->pressed == 0);
    // One gesture, one message, on release, and the tile does not light itself.
    struct desk_action fired = desk_touch_up(&model, 0, cx, cy);
    assert(fired.kind == DESK_ACT_TOGGLE && fired.widget_id == 4);
    assert(by_label(&model, "AUTO")->state == DESK_OFF);

    // Sliding off a cue before letting go sends nothing.
    assert(desk_touch_down(&model, 0, cx, cy, NULL).kind == DESK_ACT_NONE);
    desk_touch_move(&model, 0, 5, 5, NULL);
    assert(desk_touch_up(&model, 0, 5, 5).kind == DESK_ACT_NONE);

    // The show's word, not the finger's: only this lights a tile.
    desk_apply_function(&model, auto_function, 1);
    assert(by_label(&model, "AUTO")->state == DESK_ON);
    desk_apply_function(&model, charla_function, 1);
    assert(by_label(&model, "CHARLA")->state == DESK_ON);

    // Losing the link takes every claim about the rig with it.
    desk_set_link(&model, DESK_LINK_DOWN);
    assert(by_label(&model, "AUTO")->state == DESK_UNKNOWN);

    desk_set_link(&model, DESK_LINK_READY);
    desk_apply_function(&model, auto_function, 1);
    int rig_red_function = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "rig-rojo-fuego") == 0)
            rig_red_function = map.control[i].function_id;
    assert(rig_red_function >= 0);
    desk_apply_function(&model, rig_red_function, 1);
    desk_apply_master(&model, 200);

    struct desk_fonts fonts;
    assert(desk_fonts_open(&fonts, "fonts") == 0);
    struct canvas canvas = { .px = calloc(DESK_W * DESK_H, 4), .w = DESK_W, .h = DESK_H };
    assert(canvas.px);
    // The speed model as the console has it; DIRECTO's TEMPO SHOW panel
    // shows its numbers over the panel the desk paints, as dmxdesk.c does.
    struct desk_speed speed;
    desk_speed_init(&speed, &map);
    desk_speed_validate(&speed, test_surface(&console));
    desk_speed_set_link(&speed, 1, 0);
    const struct desk_placement *tempo = desk_placement_of(&model, map.count + 3);
    assert(tempo && tempo->tile == TILE_TEMPO);
    desk_paint(&canvas, &model, &fonts);
    desk_speed_paint_tempo(&canvas, &speed, 0, &fonts, tempo->x, tempo->y, tempo->w, tempo->h, 0);

    const char *out = getenv("TEST_OUT");
    char path[512];
    snprintf(path, sizeof path, "%s/desk.ppm", out ? out : "/tmp");
    write_ppm(&canvas, path);
    printf("wrote %s\n", path);

    // DIRECTO's tab wears the selected look; the link dot is green.
    struct desk_rect directo = desk_view_tab(DESK_TAB_DIRECTO), efectos = desk_view_tab(DESK_TAB_EFECTOS);
    assert(canvas.px[(directo.y + directo.h / 2) * canvas.w + directo.x + 6] == DESK_SELECTED_FILL);
    assert(canvas.px[(directo.y + directo.h / 2) * canvas.w + directo.x] == DESK_SELECTED_LINE);
    assert(canvas.px[(efectos.y + efectos.h / 2) * canvas.w + efectos.x + 6] == DESK_TILE);
    assert(canvas.px[(DESK_BAR_H / 2) * canvas.w + DESK_LINK_DOT_X + DESK_LINK_DOT / 2] == DESK_OK);

    // DIRECTO: the running state wears the selected look and the hero over
    // the states fills with the hero indigo; the rig colours carry no words,
    // the running one outlined in the accent with its colour whole inside.
    const struct desk_placement *autop = desk_placement_of(&model, (int)(by_label(&model, "AUTO") - model.control));
    assert(canvas.px[(autop->y + autop->h / 2) * canvas.w + autop->x + 4] == DESK_SELECTED_FILL);
    assert(canvas.px[(autop->y + autop->h / 2) * canvas.w + autop->x] == DESK_SELECTED_LINE);
    assert(canvas.px[(DESK_HERO_Y + DESK_HERO_H / 2) * canvas.w + DESK_HERO_X + 8] == DESK_HERO_FILL);
    assert(canvas.px[(DESK_HERO_Y + DESK_HERO_H / 2) * canvas.w + DESK_HERO_X] == DESK_ACCENT);
    for (int i = 0; i < layout.placements; i++) {
        const struct desk_placement *p = &layout.placement[i];
        if (p->page != 0 || p->tile != TILE_MINI)
            continue;
        const struct desk_control *ctl = &model.control[p->control];
        uint32_t colour = ctl->swatch[0] | 0xFF000000u;
        // No words: the whole middle band is the colour.
        for (int y = p->y + 12; y < p->y + p->h - 12; y++)
            assert(canvas.px[y * canvas.w + p->x + p->w / 2] == colour);
        int lit = ctl->state == DESK_ON;
        assert(canvas.px[(p->y + p->h / 2) * canvas.w + p->x] == (lit ? DESK_ACCENT : colour));
        assert(lit == (strcmp(ctl->label, "Rig Rojo Fuego") == 0));
    }
    // The tempo panel: the TAP target where the hit test finds it.
    int ttx, tty, ttw, tth;
    desk_speed_tempo_tap_rect(tempo->x, tempo->y, tempo->w, tempo->h, &ttx, &tty, &ttw, &tth);
    assert(ttw >= 48 && tth >= 48 && ttx + ttw <= tempo->x + tempo->w && tty + tth <= tempo->y + tempo->h);
    assert(canvas.px[(tty + 4) * canvas.w + ttx + ttw / 2] == DESK_RAISED);
    assert(canvas.px[(tempo->y + 4) * canvas.w + tempo->x + tempo->w / 2] == DESK_PANEL);
    model.color_one_shot = 1;
    desk_paint(&canvas, &model, &fonts);
    desk_speed_paint_tempo(&canvas, &speed, 0, &fonts, tempo->x, tempo->y, tempo->w, tempo->h, 0);
    snprintf(path, sizeof path, "%s/desk-colour-hits.ppm", out ? out : "/tmp");
    write_ppm(&canvas, path);

    // The COLOR page with the pick lit, the gobos' second bank, CABEZAS: the
    // EFECTOS tab lit, the family on screen lit in the column at the left,
    // the sections on their panels.
    desk_set_view(&model, desk_tab_page(&model, DESK_TAB_EFECTOS), 0);
    assert(model.page == 1);
    desk_apply_function(&model, 727, 1);
    desk_paint(&canvas, &model, &fonts);
    snprintf(path, sizeof path, "%s/desk-color.ppm", out ? out : "/tmp");
    write_ppm(&canvas, path);
    assert(canvas.px[(efectos.y + efectos.h / 2) * canvas.w + efectos.x + 6] == DESK_SELECTED_FILL);
    struct desk_rect color = desk_view_family(0, layout.families), pixels = desk_view_family(1, layout.families);
    assert(canvas.px[(color.y + 4) * canvas.w + color.x + color.w / 2] == DESK_SELECTED_FILL);
    assert(canvas.px[color.y * canvas.w + color.x + color.w / 2] == DESK_SELECTED_TOP);
    assert(canvas.px[(pixels.y + 4) * canvas.w + pixels.x + pixels.w / 2] == DESK_TILE);
    assert(canvas.px[pixels.y * canvas.w + pixels.x + pixels.w / 2] == DESK_LINE);
    for (int i = 0; i < layout.headings; i++) {
        const struct desk_heading *h = &layout.heading[i];
        if (h->page == 1 && h->bank == 0)
            assert(canvas.px[(h->y + h->h - 2) * canvas.w + h->x + h->w / 2] == DESK_PANEL);
    }
    // COLOR's picks: a colour square over the name; the running one ringed in
    // the selected line with its square whole, the rest in their keyline.
    int squares = 0;
    for (int i = 0; i < layout.placements; i++) {
        const struct desk_placement *p = &layout.placement[i];
        if (p->page != 1 || p->bank != 0 || p->tile != TILE_SWATCH)
            continue;
        const struct desk_control *ctl = &model.control[p->control];
        assert(ctl->swatches == 1);
        int lit = ctl->state == DESK_ON;
        assert(lit == (strcmp(ctl->label, "Rig Rojo Fuego") == 0));
        assert(canvas.px[(p->y + p->h / 2) * canvas.w + p->x] == (lit ? DESK_SELECTED_LINE : DESK_LINE));
        assert(canvas.px[(p->y + p->h / 2) * canvas.w + p->x + 1] == (lit ? DESK_SELECTED_LINE : DESK_TILE));
        // The square: 56 px or more, its colour whole across the middle.
        for (int dx = -26; dx <= 26; dx++)
            assert(canvas.px[(p->y + 36) * canvas.w + p->x + p->w / 2 + dx] == (ctl->swatch[0] | 0xFF000000u));
        squares++;
    }
    assert(squares == 10);
    // Running, a number and a figure are ringed in the selected line round
    // the frosted fill, and a hook wears the selected look.
    const char *running[] = { "Gobo 14", "Circulo Simultaneo", "AUTO lento" };
    for (int k = 0; k < 3; k++) {
        int index = (int)(by_label(&model, running[k]) - model.control);
        model.control[index].state = DESK_ON;
        const struct desk_placement *p = NULL;
        for (int i = 0; i < layout.placements && !p; i++)
            if (layout.placement[i].control == index)
                p = &layout.placement[i];
        assert(p);
        desk_set_view(&model, p->page, p->bank);
        desk_paint(&canvas, &model, &fonts);
        assert(canvas.px[(p->y + 4) * canvas.w + p->x + p->w / 2] == DESK_SELECTED_FILL);
        assert(canvas.px[(p->y + p->h / 2) * canvas.w + p->x] == DESK_SELECTED_LINE);
    }
    // One render per EFECTOS family and bank: desk-<page key>-<bank>.ppm.
    for (int f = 0; f < layout.families; f++) {
        int page = layout.family_page[f];
        for (int bank = 0; bank < layout.banks[page]; bank++) {
            desk_set_view(&model, page, bank);
            desk_paint(&canvas, &model, &fonts);
            snprintf(path, sizeof path, "%s/desk-%s-%d.ppm", out ? out : "/tmp", map.page[page].key, bank + 1);
            write_ppm(&canvas, path);
        }
    }
    desk_set_view(&model, 3, 0);

    // RITMO: the SPEED page's chrome and the speed model's cards over it.
    desk_set_view(&model, desk_tab_page(&model, DESK_TAB_RITMO), 0);
    assert(model.page == layout.speed_page);
    desk_paint(&canvas, &model, &fonts);
    desk_speed_paint(&canvas, &speed, &fonts);
    snprintf(path, sizeof path, "%s/desk-ritmo.ppm", out ? out : "/tmp");
    write_ppm(&canvas, path);
    struct desk_rect ritmo = desk_view_tab(DESK_TAB_RITMO);
    assert(canvas.px[(ritmo.y + ritmo.h / 2) * canvas.w + ritmo.x + 6] == DESK_SELECTED_FILL);
    // EFECTOS still remembers CABEZAS.
    assert(desk_tab_page(&model, DESK_TAB_EFECTOS) == 3);
    desk_set_view(&model, 0, 0);

    // A brief Wi-Fi loss changes the status bar without covering any cue.
    desk_set_link(&model, DESK_LINK_CONNECTING);
    desk_speed_set_link(&speed, 0, 0);
    desk_paint(&canvas, &model, &fonts);
    desk_speed_paint_tempo(&canvas, &speed, 0, &fonts, tempo->x, tempo->y, tempo->w, tempo->h, 0);
    uint32_t hero_before_overlay =
        canvas.px[(DESK_HERO_Y + DESK_HERO_H / 2) * canvas.w + DESK_HERO_X + 8];
    desk_paint_overlays(&canvas, &model, &fonts);
    assert(canvas.px[(DESK_BAR_H / 2) * canvas.w + DESK_LINK_DOT_X + DESK_LINK_DOT / 2]
           == DESK_WARN);
    assert(canvas.px[(DESK_HERO_Y + DESK_HERO_H / 2) * canvas.w + DESK_HERO_X + 8]
           == hero_before_overlay);
    assert(canvas.px[260 * canvas.w + 265] != DESK_WARN);
    uint32_t waiting_bar[DESK_W * DESK_BAR_H];
    memcpy(waiting_bar, canvas.px, sizeof waiting_bar);
    snprintf(path, sizeof path, "%s/desk-nolink.ppm", out ? out : "/tmp");
    write_ppm(&canvas, path);
    desk_set_link(&model, DESK_LINK_SYNCING);
    desk_paint(&canvas, &model, &fonts);
    desk_paint_overlays(&canvas, &model, &fonts);
    assert(memcmp(waiting_bar, canvas.px, sizeof waiting_bar) == 0);
    assert(canvas.px[260 * canvas.w + 265] != DESK_WARN);
    // A different show is an actual incompatibility and still needs its card.
    desk_set_link(&model, DESK_LINK_READY);
    model.mismatch = 1;
    desk_paint(&canvas, &model, &fonts);
    desk_paint_overlays(&canvas, &model, &fonts);
    assert(canvas.px[260 * canvas.w + 265] == DESK_WARN);
    model.mismatch = 0;

    // Six parts of one long section keep their distinguishing part numbers.
    // No label may spill into a gap, and the running dot keeps its old meaning.
    // The page is an EFECTOS family, so the pager runs right of the families.
    memset(&layout, 0, sizeof layout);
    layout.pages = 1;
    layout.tab[0] = DESK_TAB_EFECTOS;
    layout.families = 1;
    layout.family_page[0] = 0;
    layout.family_icon[0] = ICON_PALETTE;
    layout.banks[0] = DESK_MAX_BANKS;
    layout.headings = DESK_MAX_BANKS;
    strcpy(layout.title[0], "COLOR");
    for (int i = 0; i < DESK_MAX_BANKS; i++) {
        layout.heading[i].bank = i;
        layout.heading[i].part = i + 1;
        layout.heading[i].parts = DESK_MAX_BANKS;
        layout.heading[i].x = DESK_EFFECTS_X;
        layout.heading[i].y = DESK_CONTENT_Y;
        layout.heading[i].w = DESK_EFFECTS_W;
        layout.heading[i].h = 136;
        strcpy(layout.heading[i].text, "Dirección y barridos de intensidad");
    }
    layout.placements = 1;
    layout.placement[0] = (struct desk_placement){ .control = 0, .page = 0, .bank = 5,
        .x = DESK_EFFECTS_X + DESK_PANEL_PAD, .y = 92, .w = 128, .h = 88, .tile = TILE_CUE };
    desk_set_layout(&model, &layout);
    desk_set_link(&model, DESK_LINK_READY);
    model.control[0].state = DESK_ON;
    for (int fallback = 0; fallback <= 1; fallback++) {
        struct desk_fonts empty = {0};
        desk_paint(&canvas, &model, fallback ? &empty : &fonts);
        for (int i = 0; i < DESK_MAX_BANKS; i++) {
            struct desk_rect r = desk_view_pager(i, &layout, 0);
            assert(canvas.px[(r.y + 8) * canvas.w + r.x + 8] ==
                   (i == 0 ? DESK_INK : DESK_TILE));
            if (i < DESK_MAX_BANKS - 1)
                for (int y = r.y; y < r.y + r.h; y++)
                    for (int x = r.x + r.w; x < r.x + r.w + DESK_GAP; x++)
                        assert(canvas.px[y * canvas.w + x] == DESK_GLASS);
        }
        struct desk_rect last = desk_view_pager(5, &layout, 0);
        assert(canvas.px[(last.y + 8) * canvas.w + last.x + last.w - 8] == DESK_ACCENT);
        if (!fallback) {
            snprintf(path, sizeof path, "%s/desk-six-banks.ppm", out ? out : "/tmp");
            write_ppm(&canvas, path);
        }
    }

    font_close(fonts.tile);
    font_close(fonts.value);
    font_close(fonts.label);
    font_close(fonts.small);
    free(canvas.px);
    vc_free(&console);
    return 0;
}
