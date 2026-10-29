// SOURCES: desk_rebuild_model.c desk_model.c desk_placement_hit.c desk_placement_target.c desk_master_level_at.c showmap_validate.c showmap_check_priority.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c vcjson.c desk_damage_running.c desk_panel_holds.c desk_surface_find.c desk_surface_free.c desk_surface_is_button.c qlc_surface_kind.c qlc_surface_of.c
// Tabs and console refreshes keep every page's bank, including hidden pages,
// and the EFECTOS family its tab reopens.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_rebuild_model.h"
#include "desk_tab.h"
#include "test_surface.h"

int main(void) {
    struct desk_model m;
    struct show_map map;
    struct vc_doc console = {0};
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    struct desk_layout layout = { .pages = 3, .banks = {1, 3, 2} };
    desk_init(&m);
    desk_set_layout(&m, &layout);
    desk_set_view(&m, 1, 2);
    desk_set_view(&m, 0, 0);
    desk_set_view(&m, 1, -1);
    assert(m.page == 1 && m.bank == 2 && m.page_bank[1] == 2);
    desk_set_view(&m, 2, 1);
    desk_set_view(&m, 0, 0);
    desk_rebuild_model(&m, &map, test_surface(&console), &layout);
    assert(m.page == 0 && m.bank == 0);
    desk_set_view(&m, 1, -1);
    assert(m.bank == 2);
    desk_set_view(&m, 2, -1);
    assert(m.bank == 1);
    desk_rebuild_model(&m, &map, test_surface(&console), &layout);
    assert(m.page == 2 && m.bank == 1);

    // Hidden banks clamp too, and a removed page cannot retain a stale bank.
    layout.pages = 2;
    layout.banks[1] = 2;
    desk_rebuild_model(&m, &map, test_surface(&console), &layout);
    assert(m.page == 1 && m.bank == 1 && m.page_bank[1] == 1);
    assert(m.page_bank[2] == 0);
    layout.banks[1] = 0;
    desk_rebuild_model(&m, &map, test_surface(&console), &layout);
    assert(m.bank == 0 && m.page_bank[1] == 0);
    layout.banks[1] = 3;
    desk_set_layout(&m, &layout);
    desk_set_view(&m, 1, 99);
    assert(m.bank == 2 && m.page_bank[1] == 2);
    m.page_bank[1] = 0;
    desk_set_view(&m, 1, 2); // Record even on the no-change fast path.
    assert(m.page_bank[1] == 2);
    desk_set_view(&m, 1, -2);
    assert(m.bank == 0);

    // New layout installation, unlike a console refresh, forgets old banks.
    desk_set_view(&m, 1, 2);
    desk_set_layout(&m, &layout);
    assert(m.page == 0 && m.bank == 0);
    for (int i = 0; i < MAP_MAX_PAGES; i++)
        assert(m.page_bank[i] == 0);
    desk_set_view(&m, 1, -1);
    assert(m.bank == 0);

    // EFECTOS remembers its family across DIRECTO and a console refresh; a
    // new layout starts it at the first family again.
    struct desk_layout tabs = { .pages = 4, .banks = {1, 1, 1, 1},
        .tab = { DESK_TAB_DIRECTO, DESK_TAB_EFECTOS, DESK_TAB_EFECTOS, DESK_TAB_RITMO },
        .family_page = {1, 2}, .families = 2 };
    desk_set_layout(&m, &tabs);
    assert(m.effects_page == 1);
    desk_set_view(&m, 2, 0);
    desk_set_view(&m, 1, 0);
    desk_set_view(&m, 3, 0);
    desk_set_view(&m, 0, 0);
    assert(m.effects_page == 1);
    // The refresh walks every page, the last family included; it must not
    // leave that one remembered instead.
    desk_rebuild_model(&m, &map, test_surface(&console), &tabs);
    assert(m.page == 0 && m.effects_page == 1);
    desk_set_view(&m, 2, 0);
    desk_set_layout(&m, &tabs);
    assert(m.effects_page == 1);
    puts("desk_bank_memory ok");
    return 0;
}
