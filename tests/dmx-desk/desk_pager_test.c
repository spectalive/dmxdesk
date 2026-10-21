// SOURCES: desk_view_pager.c desk_pager_label.c desk_pager_caption.c desk_pager_bank_caption.c desk_view.c desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c font.c canvas.c canvas_blend.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c
// Label budgets never steal content space, even with six long Spanish captions.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_layout.h"
#include "desk_pager_bank_caption.h"
#include "desk_pager_caption.h"
#include "desk_pager_label.h"
#include "desk_tab.h"
#include "desk_view.h"
#include "showmap.h"

int main(void) {
    struct desk_layout layout = { .pages = 1 };
    for (int banks = 2; banks <= DESK_MAX_BANKS; banks++) {
        layout.banks[0] = banks;
        for (int pattern = 0; pattern < 3; pattern++) {
            layout.headings = banks;
            for (int i = 0; i < banks; i++) {
                layout.heading[i].bank = i;
                snprintf(layout.heading[i].text, MAP_CAPTION_MAX, "%s",
                         pattern == 0 ? "" : pattern == 1 && i == 0 ? "Luz" :
                         "Barridos de intensidad y dirección del escenario");
            }
            int right = DESK_CONTENT_X - DESK_GAP;
            for (int i = 0; i < banks; i++) {
                struct desk_rect r = desk_view_pager(i, &layout, 0);
                assert(r.x == right + DESK_GAP && r.w >= DESK_PAGER_MIN_W);
                assert(r.y == 528 && r.h == 56 && r.y + r.h == DESK_CONTENT_END);
                assert(desk_rect_contains(r, r.x + r.w - 1, r.y + r.h - 1));
                assert(!desk_rect_contains(r, r.x + r.w, r.y));
                right = r.x + r.w;
            }
            assert(right == DESK_CONTENT_X + DESK_CONTENT_W);
        }
    }
    // An EFECTOS family's pager spans its own content, right of the families,
    // and six long labels still fit at the minimum width.
    layout.tab[0] = DESK_TAB_EFECTOS;
    for (int banks = 2; banks <= DESK_MAX_BANKS; banks++) {
        layout.banks[0] = banks;
        int right = DESK_EFFECTS_X - DESK_GAP;
        for (int i = 0; i < banks; i++) {
            struct desk_rect r = desk_view_pager(i, &layout, 0);
            assert(r.x == right + DESK_GAP && r.w >= DESK_PAGER_MIN_W && r.y == 528);
            right = r.x + r.w;
        }
        assert(right == DESK_CONTENT_X + DESK_CONTENT_W);
    }
    layout.tab[0] = DESK_TAB_DIRECTO;
    // Equal label demand gives deterministic rounding, including the last pixel.
    layout.headings = 0;
    layout.banks[0] = 6;
    int right = DESK_CONTENT_X - DESK_GAP;
    for (int i = 0; i < 6; i++) {
        struct desk_rect r = desk_view_pager(i, &layout, 0);
        assert(r.x == right + DESK_GAP && r.w >= 138 && r.w <= 139);
        right = r.x + r.w;
    }
    assert(right == DESK_CONTENT_X + DESK_CONTENT_W);
    layout.banks[0] = 2;
    assert(desk_view_pager(0, &layout, 0).w == 432);
    assert(desk_view_pager(1, &layout, 0).x == 456);
    layout.headings = 2;
    strcpy(layout.heading[0].text, "Corto");
    strcpy(layout.heading[1].text, "Barridos de intensidad");
    assert(desk_view_pager(1, &layout, 0).w > desk_view_pager(0, &layout, 0).w);
    // UTF-8 continuation bytes must not inflate a Spanish label's allocation.
    strcpy(layout.heading[0].text, "Color");
    int plain = desk_view_pager(0, &layout, 0).w;
    strcpy(layout.heading[0].text, "Colór");
    assert(desk_view_pager(0, &layout, 0).w == plain);
    layout.heading[2] = layout.heading[0];
    strcpy(layout.heading[2].text, "A later heading");
    layout.headings = 3;
    assert(strcmp(desk_pager_label(&layout, 0, 0), "Colór") == 0);
    assert(!desk_pager_label(&layout, 0, 5)[0]);
    assert(desk_view_pager(-1, &layout, 0).w == 0);
    assert(desk_view_pager(2, &layout, 0).w == 0);
    assert(desk_view_pager(0, &layout, -1).w == 0);
    layout.banks[0] = 1;
    assert(desk_view_pager(0, &layout, 0).w == 0);

    struct font *font = font_open("fonts/Inter-SemiBold.ttf", 15);
    assert(font);
    char fit[MAP_CAPTION_MAX + 4];
    const char *label = "Dirección y barridos de intensidad";
    desk_pager_caption(font, label, 64, fit);
    assert(font_width(font, fit) <= 64 && strstr(fit, "\xe2\x80\xa6"));
    desk_pager_caption(font, label, 500, fit);
    assert(strcmp(fit, label) == 0);
    desk_pager_caption(NULL, label, 64, fit);
    assert(canvas_text_width(fit, 3) <= 64 && strstr(fit, "..."));
    char oversized[200];
    memset(oversized, 'W', sizeof oversized - 1);
    oversized[sizeof oversized - 1] = '\0';
    desk_pager_caption(font, oversized, 64, fit);
    assert(font_width(font, fit) <= 64 && strstr(fit, "\xe2\x80\xa6"));

    struct desk_model model = {0};
    model.layout.pages = 1;
    model.layout.banks[0] = DESK_MAX_BANKS;
    model.layout.headings = DESK_MAX_BANKS;
    for (int i = 0; i < DESK_MAX_BANKS; i++) {
        struct desk_heading *h = &model.layout.heading[i];
        h->bank = i;
        h->part = i + 1;
        h->parts = DESK_MAX_BANKS;
        strcpy(h->text, label);
    }
    char caption[DESK_PAGER_BANK_CAPTION_MAX];
    for (int fallback = 0; fallback <= 1; fallback++) {
        struct font *face = fallback ? NULL : font;
        for (int bank = 0; bank < DESK_MAX_BANKS; bank++) {
            struct desk_rect r = desk_view_pager(bank, &model.layout, 0);
            char part[16];
            snprintf(part, sizeof part, "%d/6", bank + 1);
            // Every possible pager width keeps the distinguishing suffix,
            // including the ASCII fallback when only the part will fit.
            for (int width = 64; width <= 346; width++) {
                desk_pager_bank_caption(&model, face, 0, bank, width, caption);
                size_t len = strlen(caption);
                assert(len >= strlen(part));
                assert(strcmp(caption + len - strlen(part), part) == 0);
                assert((face ? font_width(face, caption) :
                               canvas_text_width(caption, 3)) <= width);
            }
            desk_pager_bank_caption(&model, face, 0, bank, r.w - 64, caption);
            printf("pager six%s: %d %s; width=%d\n", fallback ? " fallback" : "",
                   bank + 1, caption, r.w);
        }
    }

    // Narrow multi-section banks name the largest tile area, not the first
    // heading or the section with the most (possibly tiny) controls.
    model.layout.headings = 2;
    model.layout.heading[0] = (struct desk_heading){ .text = "AUTO", .parts = 1 };
    model.layout.heading[1] = (struct desk_heading){ .text = "ELEGIR", .section = 1,
                                                    .part = 1, .parts = 2 };
    model.count = 3;
    model.control[2].section = 1;
    model.layout.placements = 3;
    for (int i = 0; i < 3; i++)
        model.layout.placement[i] = (struct desk_placement){ .control = i,
            .w = i == 2 ? 100 : 10, .h = 100, .tile = TILE_CUE };
    desk_pager_bank_caption(&model, font, 0, 0, 346, caption);
    assert(strcmp(caption, "AUTO \xc2\xb7 ELEGIR 1/2") == 0);
    int narrow = font_width(font, "ELEGIR 1/2 +1");
    assert(narrow < font_width(font, "AUTO \xc2\xb7 ELEGIR 1/2"));
    desk_pager_bank_caption(&model, font, 0, 0, narrow, caption);
    assert(strcmp(caption, "ELEGIR 1/2 +1") == 0);
    for (int fallback = 0; fallback <= 1; fallback++) {
        struct font *face = fallback ? NULL : font;
        desk_pager_bank_caption(&model, face, 0, 0, 64, caption);
        assert(strstr(caption, "1/2"));
        assert((face ? font_width(face, caption) : canvas_text_width(caption, 3)) <= 64);
    }
    model.layout.placement[2].w = 20; // Equal total area: stable heading order.
    desk_pager_bank_caption(&model, font, 0, 0, narrow, caption);
    assert(strcmp(caption, "AUTO +1") == 0);
    desk_pager_bank_caption(&model, font, 0, 1, 346, caption);
    assert(!caption[0]); // Headings on a different bank cannot name this one.
    strcpy(model.layout.heading[0].text, "First complete section caption");
    strcpy(model.layout.heading[1].text, "Second complete section caption");
    desk_pager_bank_caption(&model, font, 0, 0, 800, caption);
    assert(strcmp(caption, "First complete section caption \xc2\xb7 Second complete section caption 1/2") == 0);

    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2,
                               map.count + 3, &layout) == 0);
    model.layout = layout;
    model.count = map.count;
    for (int i = 0; i < map.count; i++)
        model.control[i].section = map.control[i].section;
    for (int page = 0; page < layout.pages; page++) {
        if (layout.banks[page] < 2)
            continue;
        for (int bank = 0; bank < layout.banks[page]; bank++) {
            struct desk_rect r = desk_view_pager(bank, &layout, page);
            desk_pager_bank_caption(&model, font, page, bank, r.w - 64, caption);
            printf("pager %s: %d %s; x=%d y=%d w=%d h=%d\n", layout.title[page],
                   bank + 1, caption, r.x, r.y, r.w, r.h);
            fflush(stdout);
            if (strcmp(layout.title[page], "COLOR") == 0) {
                // The expanded palette keeps ELEGIR and GOLPES readable on
                // two banks each.
                const char *expected[] = { "ELEGIR 1/2 +1", "ELEGIR 2/2", "GOLPES 1/2", "GOLPES 2/2" };
                assert(layout.banks[page] == 4);
                assert(strcmp(caption, expected[bank]) == 0);
            }
            if (strcmp(layout.title[page], "GOBOS") == 0)
                assert(strcmp(caption, bank == 0 ? "AUTO \xc2\xb7 ELEGIR 1/2" : "ELEGIR 2/2") == 0);
        }
    }
    font_close(font);
    puts("desk_pager ok");
    return 0;
}
