// SOURCES: desk_caption.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c canvas.c canvas_blend.c font.c desk_layout_resolve.c desk_show_layout.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c
// Captions measured against the bundled Inter: the show's own words on the
// tiles that will carry them, with the cuts counted rather than assumed.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_caption.h"
#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_tab.h"
#include "font.h"
#include "icon_glyph.h"
#include "showmap.h"

int main(void) {
    struct font *tile = font_open("fonts/Inter-SemiBold.ttf", 22);
    struct font *small = font_open("fonts/Inter-Regular.ttf", 16);
    assert(tile && small);
    struct desk_caption_lines l;

    assert(desk_caption_fit(tile, 186, "AUTO", &l) == 1 && strcmp(l.line[0], "AUTO") == 0 && !l.cut);
    assert(desk_caption_fit(tile, 186, "Movimiento Lissajous (S)", &l) == 2);
    assert(strcmp(l.line[0], "Movimiento") == 0 && strcmp(l.line[1], "Lissajous (S)") == 0 && !l.cut);
    assert(desk_caption_fit(tile, 186, "Rueda Colores + Barras", &l) == 2 && !l.cut);
    assert(desk_caption_fit(small, 84, "Rig Rojo", &l) == 1 && !l.cut);
    assert(desk_caption_fit(small, 84, "Rig Multicolor", &l) == 2);
    assert(strcmp(l.line[0], "Rig") == 0 && strcmp(l.line[1], "Multicolor") == 0 && !l.cut);
    assert(desk_caption_fit(small, 84, "Gobo Shake - Gobo 17", &l) == 2);
    // More words than two lines hold: the tail is cut with an ellipsis.
    assert(desk_caption_fit(small, 84, "Gobo Shake Repartido Diecisiete Largo Final", &l) == 2 && l.cut);
    assert(strstr(l.line[1], "\xe2\x80\xa6"));
    // A single word wider than the line is cut on line one.
    assert(desk_caption_fit(small, 40, "Interminable", &l) == 1 && l.cut);
    // The glyph fallback measures too.
    assert(desk_caption_fit(NULL, 60, "ABC DEF", &l) == 2 && !l.cut);
    assert(desk_caption_fit(tile, 186, "", &l) == 0);
    // The fallback font distinguishes an idle dash from an unknown state.
    assert(font_width(tile, "\xe2\x80\x94") != font_width(tile, "?"));

    // The show's own captions: no state caption is ever cut at its width;
    // picks may lose their tail at 84 px, and the count is printed.
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    int cut = 0, total = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        if (c->role == MAP_ROLE_STATE || c->role == MAP_ROLE_HOOK || c->role == MAP_ROLE_CHASE) {
            desk_caption_fit(tile, 186, c->caption, &l);
            assert(!l.cut);
            if (c->detail[0]) {
                desk_caption_fit(small, 186, c->detail, &l);
                assert(!l.cut);
            }
        } else if (c->role == MAP_ROLE_PICK) {
            total++;
            desk_caption_fit(small, 84, c->caption, &l);
            if (l.cut) {
                cut++;
                printf("cut: %s -> %s / %s\n", c->caption, l.line[0], l.line[1]);
            }
        }
    }
    printf("picks: %d, cut: %d\n", total, cut);
    assert(cut < total / 4);

    // DIRECTO at the faces the desk draws it with: a state's name in
    // SemiBold 12 and its detail in Regular 11 inside one of seven states
    // under the hero, a hold's name in SemiBold 14 inside a pad three to a
    // row, its MANTENER pill in SemiBold 12 and its length in Regular 13.
    // Nothing the operator reads there is cut.
    struct font *state = font_open("fonts/Inter-SemiBold.ttf", 12);
    struct font *detail = font_open("fonts/Inter-Regular.ttf", 11);
    struct font *hold = font_open("fonts/Inter-SemiBold.ttf", 14);
    struct font *foot = font_open("fonts/Inter-Regular.ttf", 13);
    assert(state && detail && hold && foot);
    int state_w = (DESK_HERO_W - 6 * DESK_STATE_GAP) / 7;
    int hold_w = (DESK_HITS_W - 2 * DESK_GAP) / 3;
    int states = 0, holds = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        if (c->role == MAP_ROLE_STATE) {
            states++;
            desk_caption_fit(state, state_w - 8, c->caption, &l);
            assert(!l.cut);
            desk_caption_fit(detail, state_w - 8, c->detail, &l);
            assert(!l.cut);
        } else if ((c->role == MAP_ROLE_BURST || c->role == MAP_ROLE_ACCENT) &&
                   strcmp(map.page[c->page].key, "live") == 0) {
            holds++;
            desk_caption_fit(hold, hold_w - 16, c->caption, &l);
            assert(!l.cut);
        }
    }
    assert(states == 7 && holds == 7);
    // The pill and the longest length a pad carries, and the longest reason.
    assert(font_width(state, "MANTENER") + 16 <= hold_w - 8);
    assert(font_width(foot, "m\xc3\xa1x 8 s") <= hold_w - 8);
    assert(font_width(foot, "solo en el Mac") <= hold_w - 8);
    // EFECTOS at the faces and widths its painters use (desk v3, 2026-10-19):
    // a hook's name beside its mark in SemiBold 14 on one line or two; a pick's name in SemiBold 14 under its square, mark or
    // figure; a number's caption in Regular 13. Nothing is cut.
    struct font *tile14 = font_open("fonts/Inter-SemiBold.ttf", 14);
    struct font *small13 = font_open("fonts/Inter-Regular.ttf", 13);
    assert(tile14 && small13);
    static struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    int checked = 0;
    for (int i = 0; i < layout.placements; i++) {
        const struct desk_placement *p = &layout.placement[i];
        if (layout.tab[p->page] != DESK_TAB_EFECTOS || p->control >= map.count)
            continue;
        const struct map_control *c = &map.control[p->control];
        int fits = 0;
        switch (p->tile) {
        case TILE_CUE: {
            int tw = icon_glyph(c->icon) >= 0 ? p->w - 12 - 24 - 8 - 10 : p->w - 20;
            desk_caption_fit(tile14, tw, c->caption, &l);
            fits = !l.cut;
            break;
        }
        case TILE_SWATCH:
            desk_caption_fit(tile14, p->w - 16, c->caption, &l);
            fits = !l.cut;
            break;
        case TILE_NUMBER:
            desk_caption_fit(small13, p->w - 8, c->caption, &l);
            fits = !l.cut;
            break;
        case TILE_TRAJECTORY:
            desk_caption_fit(tile14, p->w - 8, c->caption, &l);
            fits = !l.cut;
            break;
        default:
            continue;
        }
        if (!fits)
            printf("cut on EFECTOS: %s at %d px\n", c->caption, p->w);
        assert(fits);
        checked++;
    }
    assert(checked > 100);
    font_close(tile14);
    font_close(small13);
    font_close(state);
    font_close(detail);
    font_close(hold);
    font_close(foot);
    font_close(tile);
    font_close(small);
    printf("desk_caption ok\n");
    return 0;
}
