// SOURCES: desk_caption.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c canvas.c canvas_blend.c font.c
// Captions measured against the bundled Inter: the show's own words on the
// tiles that will carry them, with the cuts counted rather than assumed.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_caption.h"
#include "desk_layout.h"
#include "font.h"
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
    // SemiBold 12 and its detail in Regular 11 inside an 84 px state tile,
    // a hold's name in SemiBold 14 and its foot in Regular 13 inside one of
    // seven equal GOLPES tiles. Nothing the operator reads there is cut.
    struct font *state = font_open("fonts/Inter-SemiBold.ttf", 12);
    struct font *detail = font_open("fonts/Inter-Regular.ttf", 11);
    struct font *hold = font_open("fonts/Inter-SemiBold.ttf", 14);
    struct font *foot = font_open("fonts/Inter-Regular.ttf", 13);
    assert(state && detail && hold && foot);
    int hold_w = (DESK_CONTENT_W - 2 * DESK_PANEL_PAD - 6 * DESK_GAP) / 7;
    int states = 0, holds = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        if (c->role == MAP_ROLE_STATE) {
            states++;
            desk_caption_fit(state, DESK_STATE_W - 8, c->caption, &l);
            assert(!l.cut);
            desk_caption_fit(detail, DESK_STATE_W - 8, c->detail, &l);
            assert(!l.cut);
        } else if ((c->role == MAP_ROLE_BURST || c->role == MAP_ROLE_ACCENT) &&
                   strcmp(map.page[c->page].key, "live") == 0) {
            holds++;
            desk_caption_fit(hold, hold_w - 16, c->caption, &l);
            assert(!l.cut);
        }
    }
    assert(states == 7 && holds == 7);
    // The longest foot a hold carries: "pulsa · máx 8 s".
    assert(font_width(foot, "pulsa \xc2\xb7 m\xc3\xa1x 8 s") <= hold_w - 8);
    font_close(state);
    font_close(detail);
    font_close(hold);
    font_close(foot);
    font_close(tile);
    font_close(small);
    printf("desk_caption ok\n");
    return 0;
}
