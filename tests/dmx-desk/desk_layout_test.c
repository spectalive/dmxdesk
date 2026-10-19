// SOURCES: desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c desk_family_icon.c icon_glyph.c
// The resolver over the real map: every control placed, nothing overlapping,
// nothing outside the content area, the banks as the arithmetic yields them,
// three tabs, six EFECTOS families, and every flowed section on its panel.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_show_layout.h"
#include "desk_tab.h"
#include "icon_data.h"
#include "showmap.h"

static int overlap(const struct desk_placement *a, const struct desk_placement *b) {
    return a->x < b->x + b->w && b->x < a->x + a->w && a->y < b->y + b->h && b->y < a->y + a->h;
}

static const struct desk_placement *find(const struct desk_layout *l, int control, int page) {
    for (int i = 0; i < l->placements; i++)
        if (l->placement[i].control == control && l->placement[i].page == page)
            return &l->placement[i];
    return NULL;
}

static int headings_named(const struct desk_layout *l, int page, const char *text) {
    int n = 0;
    for (int i = 0; i < l->headings; i++)
        if (l->heading[i].page == page && strcmp(l->heading[i].text, text) == 0)
            n++;
    return n;
}

static int control_named(const struct show_map *map, const char *caption) {
    for (int i = 0; i < map->count; i++)
        if (strcmp(map->control[i].caption, caption) == 0)
            return i;
    return -1;
}

int main(void) {
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    int master = map.count, panic = map.count + 1;
    struct desk_layout l;
    assert(desk_layout_resolve(&map, master, panic, map.count + 2, map.count + 3, &l) == 0);
    assert(l.pages == 8 && l.speed_page == 7 && strcmp(l.title[7], "SPEED") == 0);
    // Three tabs: the live page is DIRECTO, SPEED is RITMO, and the six
    // families, in map order, titled by the map and marked by their keys.
    int per_tab[DESK_TABS] = {0};
    for (int page = 0; page < l.pages; page++)
        per_tab[l.tab[page]]++;
    assert(l.tab[0] == DESK_TAB_DIRECTO && l.tab[7] == DESK_TAB_RITMO);
    assert(per_tab[DESK_TAB_DIRECTO] == 1 && per_tab[DESK_TAB_RITMO] == 1 &&
           per_tab[DESK_TAB_EFECTOS] == 6);
    const char *families[] = { "COLOR", "PIXELES", "CABEZAS", "GOBOS", "PRISMA", "CONTROL" };
    const int marks[] = { ICON_PALETTE, ICON_GRID, ICON_MOVING_HEAD, ICON_GOBO, ICON_PRISM,
                          ICON_SLIDERS };
    assert(l.families == 6);
    for (int i = 0; i < 6; i++) {
        assert(l.family_page[i] == i + 1 && l.tab[i + 1] == DESK_TAB_EFECTOS);
        assert(strcmp(l.title[l.family_page[i]], families[i]) == 0);
        assert(strcmp(map.page[l.family_page[i]].title, families[i]) == 0);
        assert(l.family_icon[i] == marks[i]);
    }

    // Every control has a placement on its own page.
    for (int i = 0; i < map.count; i++)
        assert(find(&l, i, map.control[i].page));
    // SHOW: the states 111x88 seven across, the hits 128x104 holds, the fog
    // 160x88, the rhythms as segments, the rig colours as minis, the tempo.
    const struct desk_placement *a = find(&l, control_named(&map, "AUTO"), 0);
    assert(a->tile == TILE_STATE && a->w == 111 && a->h == 88 && a->x == DESK_CONTENT_X);
    const struct desk_placement *ch = find(&l, control_named(&map, "CHARLA"), 0);
    assert(ch->y == a->y && ch->x == a->x + 111 + 8);
    const struct desk_placement *h1 = find(&l, control_named(&map, "cada 1 min"), 0);
    const struct desk_placement *h4 = find(&l, control_named(&map, "cada 8 min"), 0);
    assert(h1 && h4 && h1->tile == TILE_SEGMENT && h1->y == h4->y && h4->x + h4->w <= DESK_MASTER_X);
    const struct desk_placement *off = find(&l, map.count + 2, 0);
    assert(off && off->tile == TILE_SEGMENT && off->x < h1->x);
    const struct desk_placement *flash = find(&l, control_named(&map, "FLASH"), 0);
    assert(flash && flash->tile == TILE_HOLD && flash->h == 104 && flash->w == 128);
    const struct desk_placement *fog = find(&l, control_named(&map, "HUMO YA"), 0);
    assert(fog && fog->tile == TILE_FOG && fog->w == 160);
    // color-beam vanished from LIVE. The new colores-completos hook on
    // COLOR retains the colour-toggle role, as a cue beside the other hooks.
    int colours = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "colores-completos") == 0)
            colours = i;
    assert(colours >= 0);
    const struct desk_placement *hook = find(&l, colours, map.control[colours].page);
    assert(hook && hook->tile == TILE_CUE && hook->w == 200 && hook->h == 64);
    const struct desk_placement *mini = find(&l, control_named(&map, "Rig Rojo"), 0);
    assert(mini && mini->tile == TILE_MINI && mini->w == 52);
    const struct desk_placement *tempo = find(&l, map.count + 3, 0);
    assert(tempo && tempo->tile == TILE_TEMPO && tempo->x + tempo->w == DESK_CONTENT_X + DESK_CONTENT_W);
    assert(strcmp(l.title[0], "SHOW") == 0 && l.banks[0] == 1);
    // Fijo names SHOW's fixed accent toggles; this map has none, so no empty
    // heading sits beside the hits (2026-10-19, the host desk.ppm showed one).
    assert(headings_named(&l, 0, "Golpes \xc2\xb7 mientras pulses") == 1);
    assert(headings_named(&l, 0, "Fijo") == 0);
    {
        // Turn one LIVE accent into a toggle: it is placed as a cue and the
        // heading comes back over it.
        int live = desk_show_live_page(&map);
        assert(live >= 0);
        const struct map_section *accents = NULL;
        for (int s = 0; s < map.page[live].sections; s++)
            if (strcmp(map.page[live].section[s].key, "accents") == 0)
                accents = &map.page[live].section[s];
        assert(accents && accents->count > 0);
        static struct show_map toggled;
        static struct desk_layout show;
        toggled = map;
        toggled.control[accents->first].role = MAP_ROLE_TOGGLE;
        memset(&show, 0, sizeof show);
        assert(desk_show_compose(&toggled, 0, -1, -1, &show) == 0);
        assert(headings_named(&show, 0, "Fijo") == 1);
        const struct desk_placement *cue = find(&show, accents->first, 0);
        assert(cue && cue->tile == TILE_CUE);
        for (int i = 0; i < show.headings; i++)
            if (strcmp(show.heading[i].text, "Fijo") == 0)
                assert(show.heading[i].x == cue->x && show.heading[i].y < cue->y);
    }
    // COLOR keeps its picks 128x88, five to a row inside their panel right of
    // the families, and its colour hits as holds.
    const struct desk_placement *rojo = find(&l, control_named(&map, "Rig Rojo"), 1);
    assert(rojo && rojo->tile == TILE_SWATCH && rojo->w == 128 && rojo->h == 88);
    assert(rojo->x == DESK_EFFECTS_X + DESK_PANEL_PAD);
    int in_row = 0;
    for (int i = 0; i < l.placements; i++)
        if (l.placement[i].page == 1 && l.placement[i].bank == rojo->bank &&
            l.placement[i].y == rojo->y && l.placement[i].tile == TILE_SWATCH)
            in_row++;
    assert(in_row == 5);
    const struct desk_placement *hit = find(&l, control_named(&map, "ROJO"), 1);
    assert(hit && hit->tile == TILE_HOLD && hit->h == 104);

    // Bounds and overlaps, per page and bank; the master and the panic button
    // once per bank.
    for (int page = 0; page < l.pages; page++) {
        assert(l.banks[page] >= 1 && l.banks[page] <= DESK_MAX_BANKS);
        int end = l.banks[page] > 1 ? DESK_CONTENT_END - 56 : DESK_CONTENT_END;
        for (int bank = 0; bank < l.banks[page]; bank++) {
            int masters = 0, panics = 0;
            for (int i = 0; i < l.placements; i++) {
                const struct desk_placement *p = &l.placement[i];
                if (p->page != page || p->bank != bank)
                    continue;
                if (p->tile == TILE_MASTER) { masters++; continue; }
                if (p->tile == TILE_PANIC) { panics++; continue; }
                assert(p->x >= DESK_GRID_X && p->x + p->w <= DESK_MASTER_X);
                assert(p->y >= DESK_GRID_Y && p->y + p->h <= end);
                // A family's tiles keep clear of the families' column, inside
                // a panel of their own section on the same bank.
                if (l.tab[page] == DESK_TAB_EFECTOS) {
                    assert(p->x >= DESK_EFFECTS_X + DESK_PANEL_PAD);
                    int panels = 0;
                    for (int h = 0; h < l.headings; h++) {
                        const struct desk_heading *hd = &l.heading[h];
                        if (hd->page == page && hd->bank == bank &&
                            hd->section == map.control[p->control].section &&
                            p->x >= hd->x + DESK_PANEL_PAD && p->x + p->w <= hd->x + hd->w - DESK_PANEL_PAD &&
                            p->y >= hd->y + DESK_PANEL_PAD + 16 && p->y + p->h <= hd->y + hd->h - DESK_PANEL_PAD)
                            panels++;
                    }
                    assert(panels == 1);
                }
                for (int j = i + 1; j < l.placements; j++) {
                    const struct desk_placement *q = &l.placement[j];
                    if (q->page == page && q->bank == bank && q->tile != TILE_MASTER &&
                        q->tile != TILE_PANIC)
                        assert(!overlap(p, q));
                }
            }
            assert(masters == 1 && panics == 1);
        }
    }
    // Panels: a family's never overlap and stay in its content; SHOW's
    // composed headings are bare, with no panel.
    for (int i = 0; i < l.headings; i++) {
        const struct desk_heading *a = &l.heading[i];
        if (l.tab[a->page] != DESK_TAB_EFECTOS) {
            assert(a->h == 0);
            continue;
        }
        int end = l.banks[a->page] > 1 ? DESK_PAGER_Y - DESK_GAP : DESK_CONTENT_END;
        assert(a->h > 0 && a->x == DESK_EFFECTS_X && a->x + a->w == DESK_MASTER_X - 16);
        assert(a->y >= DESK_CONTENT_Y && a->y + a->h <= end);
        for (int j = i + 1; j < l.headings; j++) {
            const struct desk_heading *b = &l.heading[j];
            if (b->page == a->page && b->bank == a->bank)
                assert(a->y + a->h + DESK_GAP <= b->y || b->y + b->h + DESK_GAP <= a->y);
        }
    }
    // Headings: the gobo picks are split across banks with part/parts.
    int gobo_parts = 0;
    for (int i = 0; i < l.headings; i++) {
        const struct desk_heading *h = &l.heading[i];
        if (h->page == 4 && strcmp(h->text, "ELEGIR") == 0) {
            assert(h->parts >= 2 && h->part >= 1 && h->part <= h->parts);
            gobo_parts++;
        }
    }
    assert(gobo_parts >= 2);
    printf("banks per page:");
    for (int page = 0; page < l.pages; page++)
        printf(" %s=%d", map.page[page].key, l.banks[page]);
    printf("\nplacements %d headings %d\n", l.placements, l.headings);
    printf("desk_layout ok\n");
    return 0;
}
