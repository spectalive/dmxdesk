// SOURCES: desk_caption_number.c desk_pick_numbered.c desk_trajectory_of.c icon_glyph.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c
// What a pick shows in its mark's place, read from captions alone
// (2026-10-19, desk v3 EFECTOS): the number a caption ends in, which picks
// of a section read as their number, and the heads' figure a caption names.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_caption_number.h"
#include "desk_pick_numbered.h"
#include "desk_trajectory_of.h"
#include "icon_data.h"
#include "icon_glyph.h"
#include "showmap.h"

static const char *number_of(const char *caption) {
    const char *n = desk_caption_number(caption);
    return n ? n : "-";
}

int main(void) {
    // The trailing number: digits, joined by "y", starting and ending on digits.
    assert(strcmp(number_of("Gobo 14"), "14") == 0);
    assert(strcmp(number_of("Effect 5"), "5") == 0);
    assert(strcmp(number_of("1"), "1") == 0);
    assert(strcmp(number_of("1 y 3"), "1 y 3") == 0);
    assert(strcmp(number_of("Gobo Repartido 3"), "3") == 0);
    assert(strcmp(number_of("Gobo Shake - Gobo 9"), "9") == 0);
    assert(strcmp(number_of("y 3"), "3") == 0);
    assert(strcmp(number_of("Paneles Manual"), "-") == 0);
    assert(strcmp(number_of("Insert Prism"), "-") == 0);
    assert(strcmp(number_of("Gobo 3b"), "-") == 0);
    assert(strcmp(number_of("3 y"), "-") == 0);
    assert(strcmp(number_of(""), "-") == 0);
    assert(desk_caption_number(NULL) == NULL);

    // The rule over the real map: a section's most shared stem wins.
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    int numbered = 0, gobo_numbers = 0, prism_numbers = 0, pixel_numbers = 0;
    for (int p = 0; p < map.pages; p++)
        for (int s = 0; s < map.page[p].sections; s++) {
            const struct map_section *sec = &map.page[p].section[s];
            for (int i = sec->first; i < sec->first + sec->count; i++) {
                const char *caption = map.control[i].caption;
                if (!desk_pick_numbered(&map, sec, i))
                    continue;
                numbered++;
                assert(map.control[i].role == MAP_ROLE_PICK);
                if (strcmp(map.page[p].key, "gobos") == 0) {
                    assert(strncmp(caption, "Gobo ", 5) == 0 && caption[5] >= '0' && caption[5] <= '9');
                    gobo_numbers++;
                } else if (strcmp(map.page[p].key, "prism") == 0) {
                    assert(caption[0] >= '0' && caption[0] <= '9');
                    prism_numbers++;
                } else {
                    assert(strcmp(map.page[p].key, "pixels") == 0 && strncmp(caption, "Effect ", 7) == 0);
                    pixel_numbers++;
                }
            }
        }
    assert(gobo_numbers == 17 && prism_numbers == 6 && pixel_numbers == 12 && numbered == 35);

    // A lone number under its stem is not a numbered section, and a tie goes
    // to the stem met first.
    struct show_map small = map;
    struct map_section sec = { .first = 0, .count = 3 };
    const char *lone[] = { "Strobe 2", "Wash", "Beam" };
    for (int i = 0; i < 3; i++) {
        snprintf(small.control[i].caption, sizeof small.control[i].caption, "%s", lone[i]);
        small.control[i].role = MAP_ROLE_PICK;
    }
    assert(!desk_pick_numbered(&small, &sec, 0));
    sec.count = 4;
    const char *tie[] = { "A 1", "B 1", "A 2", "B 2" };
    for (int i = 0; i < 4; i++) {
        snprintf(small.control[i].caption, sizeof small.control[i].caption, "%s", tie[i]);
        small.control[i].role = MAP_ROLE_PICK;
    }
    assert(desk_pick_numbered(&small, &sec, 0) && desk_pick_numbered(&small, &sec, 2));
    assert(!desk_pick_numbered(&small, &sec, 1) && !desk_pick_numbered(&small, &sec, 3));
    // Only picks are numbered.
    small.control[0].role = MAP_ROLE_HOOK;
    assert(!desk_pick_numbered(&small, &sec, 0));

    // Trajectories: the leading shape word, then the mode word.
    assert(desk_trajectory_of("Circulo") == ICON_TRAJ_CIRCLE);
    assert(desk_trajectory_of("Ocho Simultaneo") == ICON_TRAJ_EIGHT_SIMULTANEOUS);
    assert(desk_trajectory_of("Linea Alternado") == ICON_TRAJ_LINE_ALTERNATE);
    assert(desk_trajectory_of("Diamante") == ICON_TRAJ_DIAMOND);
    assert(desk_trajectory_of("cuadrado simultaneo") == ICON_TRAJ_SQUARE_SIMULTANEOUS);
    assert(desk_trajectory_of("Hoja Alternado") == ICON_TRAJ_LEAF_ALTERNATE);
    assert(desk_trajectory_of("Lissajous Simult\xc3\xa1neo") == ICON_TRAJ_LISSAJOUS_SIMULTANEOUS);
    assert(desk_trajectory_of("C\xc3\xadrculo") == ICON_TRAJ_CIRCLE);
    assert(desk_trajectory_of("Circulo Lento") == ICON_TRAJ_CIRCLE);
    assert(desk_trajectory_of("Circulos") == -1);
    assert(desk_trajectory_of("Ola Vertical") == -1);
    assert(desk_trajectory_of("Movimiento Circulo") == -1);
    assert(desk_trajectory_of("") == -1 && desk_trajectory_of(NULL) == -1);

    // Every CABEZAS pick in the shipped map draws a figure or wears its glyph.
    int figures = 0, glyphs = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        if (c->role != MAP_ROLE_PICK || strcmp(map.page[c->page].key, "heads") != 0)
            continue;
        if (desk_trajectory_of(c->caption) >= 0) {
            figures++;
        } else {
            assert(icon_glyph(c->icon) >= 0);
            printf("glyph: %s\n", c->caption);
            glyphs++;
        }
    }
    assert(figures == 21 && glyphs == 5);
    printf("desk_pick_mark ok\n");
    return 0;
}
