// SOURCES: desk_view.c desk_view_pager.c desk_pager_label.c desk_input.c desk_pager_hit.c desk_model.c desk_placement_hit.c desk_placement_target.c desk_master_level_at.c desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c desk_tab_page.c desk_view_family.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_panel_holds.c
// The three tabs, EFECTOS' families, the bank pills and the lock target: where
// they are, and which one a finger lands on. A tap completes only on the
// entry it started on.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_input.h"
#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_model.h"
#include "desk_tab.h"
#include "desk_tab_page.h"
#include "desk_view.h"
#include "desk_view_family.h"
#include "showmap.h"

static struct touch_event ev(enum touch_kind kind, int slot, int x, int y) {
    struct touch_event e = { kind, slot, (float)x, (float)y, 0 };
    return e;
}

int main(void) {
    struct desk_rect e0 = desk_view_tab(0), e1 = desk_view_tab(1), e2 = desk_view_tab(2);
    assert(e0.x == 16 && e0.y == 0 && e0.w == 136 && e0.h == 48 && e0.y + e0.h <= DESK_BAR_H);
    assert(e1.x == e0.x + 144 && e2.x == e1.x + 144);
    assert(e2.x + e2.w + 8 <= DESK_STATUS_END);        // the status word keeps its run
    struct desk_rect lock = desk_view_lock_target(), gear = desk_view_gear();
    assert(lock.w == 48 && lock.h == 48 && lock.x + lock.w <= DESK_W && lock.y == 0);
    assert(gear.x + gear.w <= lock.x);


    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    struct desk_model m;
    desk_init(&m);
    struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    desk_set_layout(&m, &layout);

    int index;
    assert(desk_input_target(&m, 10, 2, &index) == TARGET_NONE);            // the bar's margin
    assert(desk_input_target(&m, e1.x + 10, e1.y + 10, &index) == TARGET_RAIL && index == 1);
    assert(desk_input_target(&m, desk_view_tab(3).x + 10, 20, &index) != TARGET_RAIL);  // no fourth tab
    // DIRECTO opens the live page, RITMO the speed page, EFECTOS the first
    // family until another has been on screen, then that one, whatever the
    // tab on screen is meanwhile.
    assert(desk_tab_page(&m, DESK_TAB_DIRECTO) == 0);
    assert(desk_tab_page(&m, DESK_TAB_RITMO) == layout.speed_page && layout.speed_page == 7);
    assert(layout.families == 6 && desk_tab_page(&m, DESK_TAB_EFECTOS) == layout.family_page[0]);
    desk_set_view(&m, layout.family_page[3], 0);
    desk_set_view(&m, 0, 0);
    assert(desk_tab_page(&m, DESK_TAB_EFECTOS) == layout.family_page[3]);
    desk_set_view(&m, layout.speed_page, 0);
    assert(desk_tab_page(&m, DESK_TAB_EFECTOS) == layout.family_page[3]);
    // The families: a column of six 124x64 targets on a family's page only,
    // and only while the settings are closed.
    struct desk_rect f0 = desk_view_family(0, 6), f5 = desk_view_family(5, 6);
    assert(f0.x == 16 && f0.y == DESK_CONTENT_Y && f0.w == 124 && f0.h == 64);
    assert(f5.y == f0.y + 5 * 72 && f5.y + f5.h <= DESK_CONTENT_END);
    assert(f0.x + f0.w + DESK_GAP == DESK_EFFECTS_X);
    assert(desk_view_family(6, 6).w == 0 && desk_view_family(0, 0).w == 0);
    // However many families a map has, each keeps a target of 48 px or more.
    for (int n = 1; n <= MAP_MAX_PAGES; n++) {
        struct desk_rect last = desk_view_family(n - 1, n);
        assert(last.h >= 48 && last.y + last.h <= DESK_CONTENT_END);
    }
    desk_set_view(&m, 0, 0);
    assert(desk_input_target(&m, f5.x + 10, f5.y + 10, &index) == TARGET_CONTENT);
    desk_set_view(&m, layout.family_page[2], 0);
    assert(desk_input_target(&m, f5.x + 10, f5.y + 10, &index) == TARGET_FAMILY && index == 5);
    assert(desk_input_target(&m, f0.x + 10, f0.y + f0.h + 2, &index) == TARGET_CONTENT); // the gap
    m.setup_open = 1;
    assert(desk_input_target(&m, f5.x + 10, f5.y + 10, &index) == TARGET_CONTENT);
    m.setup_open = 0;
    desk_set_view(&m, layout.speed_page, 0);
    assert(desk_input_target(&m, f5.x + 10, f5.y + 10, &index) == TARGET_CONTENT);
    desk_set_view(&m, 0, 0);
    assert(desk_input_target(&m, lock.x + 5, lock.y + 5, &index) == TARGET_LOCK);
    assert(desk_input_target(&m, gear.x + 5, gear.y + 5, &index) == TARGET_NONE);  // the gear is the caller's
    assert(desk_input_target(&m, 400, 300, &index) == TARGET_CONTENT);
    // Setup is modal over all painted tabs; lock remains available.
    m.setup_open=1;
    for(int i=0;i<DESK_TABS;i++) {
        struct desk_rect tab=desk_view_tab(i);
        assert(desk_input_target(&m,tab.x+20,20,&index)==TARGET_NONE && index==-1);
    }
    assert(desk_input_target(&m,lock.x+5,5,&index)==TARGET_LOCK);
    m.setup_open=0;
    for(int y=-1;y<=DESK_H;y++)for(int x=-1;x<=DESK_W;x++)
        if(x==-1||x==DESK_W||y==-1||y==DESK_H)
            assert(desk_input_target(&m,x,y,&index)==TARGET_NONE && index==-1);
    // A page with more than one bank has a pager at the bottom right.
    int paged = -1;
    for (int i = 0; i < layout.pages; i++)
        if (layout.banks[i] > 1) { paged = i; break; }
    assert(paged >= 0);
    desk_set_view(&m, paged, 0);
    struct desk_rect p1 = desk_view_pager(1, &layout, paged);
    assert(desk_input_target(&m, p1.x + 5, p1.y + 5, &index) == TARGET_BANK && index == 1);
    // A page with one bank has no pager: the same point is content.
    desk_set_view(&m, 6, 0);
    assert(layout.banks[6] == 1);
    assert(desk_input_target(&m, p1.x + 5, p1.y + 5, &index) == TARGET_CONTENT);
    desk_set_view(&m, 0, 0);

    struct desk_input in;
    desk_input_init(&in);
    // Down on DIRECTO, up on EFECTOS: nothing.
    struct touch_event d = ev(TOUCH_DOWN, 0, e0.x + 20, e0.y + 10);
    assert(desk_input_feed(&in, &m, &d, &index) == TARGET_RAIL && index == -1);
    struct touch_event u = ev(TOUCH_UP, 0, e1.x + 20, e1.y + 10);
    assert(desk_input_feed(&in, &m, &u, &index) == TARGET_RAIL && index == -1);
    // Down and up on EFECTOS: tab 1.
    d = ev(TOUCH_DOWN, 0, e1.x + 20, e1.y + 10);
    desk_input_feed(&in, &m, &d, &index);
    u = ev(TOUCH_UP, 0, e1.x + 20, e1.y + 10);
    assert(desk_input_feed(&in, &m, &u, &index) == TARGET_RAIL && index == 1);
    // A content contact stays content through its moves.
    d = ev(TOUCH_DOWN, 1, 400, 300);
    assert(desk_input_feed(&in, &m, &d, &index) == TARGET_CONTENT);
    struct touch_event mv = ev(TOUCH_MOVE, 1, 20, e1.y + 10);
    assert(desk_input_feed(&in, &m, &mv, &index) == TARGET_CONTENT);
    u = ev(TOUCH_UP, 1, 20, e1.y + 10);
    assert(desk_input_feed(&in, &m, &u, &index) == TARGET_CONTENT && index == -1);
    // Every target kind tolerates a roll, but never transfers to a neighbour:
    // a bank, a tab, and a family, whose neighbour is the one under it.
    desk_set_view(&m, paged, 0);
    assert(layout.tab[paged] == DESK_TAB_EFECTOS);
    for (int rail = 0; rail <= 2; rail++) {
        struct desk_rect first = rail == 1 ? desk_view_tab(0) : rail == 2 ? desk_view_family(0, 6)
                               : desk_view_pager(0, &layout, paged);
        struct desk_rect second = rail == 1 ? desk_view_tab(1) : rail == 2 ? desk_view_family(1, 6)
                                : desk_view_pager(1, &layout, paged);
        enum desk_target target = rail == 1 ? TARGET_RAIL : rail == 2 ? TARGET_FAMILY : TARGET_BANK;
        const int lifts[][3] = {
            {first.x - 4, first.y + 20, 0},
            {first.x + 20, first.y + first.h + 4, 0},
            {first.x - DESK_INPUT_SLOP, first.y + 20, 0},
            {first.x - DESK_INPUT_SLOP - 1, first.y + 20, -1},
            {second.x, second.y + 20, -1},
            {second.x + second.w / 2, second.y + 20, -1},
            {first.x + 20, first.y + first.h + 40, -1},
        };
        for (unsigned i = 0; i < sizeof lifts / sizeof lifts[0]; i++) {
            d = ev(TOUCH_DOWN, 0, first.x + 20, first.y + 20);
            assert(desk_input_feed(&in, &m, &d, &index) == target && index == -1);
            u = ev(TOUCH_UP, 0, lifts[i][0], lifts[i][1]);
            assert(desk_input_feed(&in, &m, &u, &index) == target && index == lifts[i][2]);
            assert(desk_input_feed(&in, &m, &u, &index) == TARGET_NONE && index == -1);
        }
        d = ev(TOUCH_DOWN, 0, first.x + 20, first.y + 20);
        desk_input_feed(&in, &m, &d, &index);
        u = ev(TOUCH_CANCEL, 0, first.x + 20, first.y + 20);
        assert(desk_input_feed(&in, &m, &u, &index) == target && index == -1);
    }
    // A bank pressed before another finger changes tab must not select a
    // bank on that other page, even when the rectangles happen to coincide.
    p1 = desk_view_pager(1, &layout, paged);
    d = ev(TOUCH_DOWN, 0, p1.x + 20, p1.y + 20);
    desk_input_feed(&in, &m, &d, &index);
    desk_set_view(&m, 0, 0);
    u = ev(TOUCH_UP, 0, p1.x + 20, p1.y + 20);
    assert(desk_input_feed(&in, &m, &u, &index) == TARGET_BANK && index == -1);
    // Release slop ends at the physical glass; the next contact still works.
    desk_set_view(&m,paged,0);
    d=ev(TOUCH_DOWN,0,p1.x+20,DESK_H-1);
    assert(desk_input_feed(&in,&m,&d,&index)==TARGET_BANK);
    u=ev(TOUCH_UP,0,p1.x+20,DESK_H);
    assert(desk_input_feed(&in,&m,&u,&index)==TARGET_BANK && index==-1);
    d=ev(TOUCH_DOWN,0,e0.x+20,0);d.y=-0.5f;
    assert(desk_input_feed(&in,&m,&d,&index)==TARGET_NONE);
    d.y=0;
    assert(desk_input_feed(&in,&m,&d,&index)==TARGET_RAIL);
    u=ev(TOUCH_UP,0,e0.x+20,0);u.y=-0.5f;
    assert(desk_input_feed(&in,&m,&u,&index)==TARGET_RAIL && index==-1);
    printf("desk_view ok\n");
    return 0;
}
