// SOURCES: desk_layout_resolve.c desk_show_layout.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c
// The resolver over the real map: every control placed, nothing overlapping,
// nothing outside the content area, the banks as the arithmetic yields them,
// three tabs, six EFECTOS families, and every flowed section on its panel.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_show_layout.h"
#include "desk_show_slot.h"
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

static const struct desk_heading *heading_named(const struct desk_layout *l, int page, const char *text) {
    for (int i = 0; i < l->headings; i++)
        if (l->heading[i].page == page && strcmp(l->heading[i].text, text) == 0)
            return &l->heading[i];
    return NULL;
}

// Inside the panel's padding, under its heading.
static int inside(const struct desk_placement *p, const struct desk_heading *h) {
    return p->x >= h->x + DESK_PANEL_PAD && p->x + p->w <= h->x + h->w - DESK_PANEL_PAD &&
           p->y >= h->y + DESK_PANEL_PAD && p->y + p->h <= h->y + h->h - DESK_PANEL_PAD;
}

static int in_panel(const struct desk_layout *l, const struct desk_heading *h) {
    int n = 0;
    for (int i = 0; i < l->placements; i++)
        if (l->placement[i].page == h->page && l->placement[i].bank == h->bank &&
            inside(&l->placement[i], h))
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
    // DIRECTO, the hero layout: SALA AHORA's hero over the seven states at
    // the left, EFECTOS · MIENTRAS PULSES at the right, and the bottom row of
    // HUMO AMBIENTE, COLOR DEL RIG and TEMPO SHOW.
    const struct desk_heading *room = heading_named(&l, 0, "SALA AHORA");
    const struct desk_heading *hits = heading_named(&l, 0, "EFECTOS \xc2\xb7 MIENTRAS PULSES");
    const struct desk_heading *haze = heading_named(&l, 0, "HUMO AMBIENTE");
    const struct desk_heading *rig = heading_named(&l, 0, "COLOR DEL RIG");
    assert(room && hits && haze && rig);
    assert(room->kind == DESK_PANEL_ROOM && rig->kind == DESK_PANEL_NAMES_RUNNING);
    assert(hits->kind == DESK_PANEL_PLAIN && haze->kind == DESK_PANEL_PLAIN);
    // The compact room readout sits below the mode buttons.
    assert(room->x == DESK_CONTENT_X && room->y == DESK_HERO_Y && room->w == DESK_HERO_W &&
           room->h == DESK_HERO_H && in_panel(&l, room) == 0);
    // Seven modes are the first row, above the room readout.
    const char *states[] = { "AUTO", "CHARLA", "TRANQUILO", "FIESTA", "LOCURA", "BLANCO TOTAL", "TODO NEGRO" };
    const struct desk_placement *prev = NULL;
    for (int k = 0; k < 7; k++) {
        const struct desk_placement *st = find(&l, control_named(&map, states[k]), 0);
        assert(st && st->tile == TILE_STATE && st->w >= 68 && st->h == DESK_STATE_H);
        assert(st->y == DESK_STATE_Y);
        if (prev)
            assert(st->y == prev->y && st->x == prev->x + prev->w + DESK_STATE_GAP && abs(st->w - prev->w) <= 1);
        else
            assert(st->x == room->x);
        prev = st;
    }
    assert(prev->x + prev->w == room->x + room->w);
    assert(prev->y + prev->h + DESK_GAP == room->y);
    // The holds: a heading line at the right of the hero, then six pads three
    // to a row, the light ones in map order and HUMO YA last, and HUMO VERT
    // as one wide pad under them that closes the column level with the states.
    assert(hits->h == 0 && hits->x == room->x + room->w + DESK_GAP && hits->y == DESK_HITS_Y &&
           hits->x + hits->w == DESK_CONTENT_X + DESK_CONTENT_W);
    const char *pads[] = { "FLASH", "FLASH LENTO", "FLASH COLOR", "STROBO", "STROBO SUAVE", "HUMO YA" };
    for (int k = 0; k < 6; k++) {
        const struct desk_placement *g = find(&l, control_named(&map, pads[k]), 0);
        assert(g && g->tile == (k == 5 ? TILE_FOG : TILE_HOLD) && g->h >= 48 && g->w >= 88);
        assert(g->y == DESK_HITS_TOP + (k / 3) * (g->h + DESK_GAP));
        int col = k % 3, w;
        assert(g->x == desk_show_slot(hits->x, hits->w, 3, DESK_GAP, col, &w) && g->w == w);
    }
    const struct desk_placement *vert = find(&l, control_named(&map, "HUMO VERT"), 0);
    assert(vert && vert->tile == TILE_FOG && vert->x == hits->x && vert->w == hits->w);
    assert(vert->h == DESK_HOLD_WIDE_H && vert->y + vert->h == DESK_HITS_END);
    // The colour grid is below the modes and the timing selector below effects.
    assert(rig->x == DESK_CONTENT_X && rig->y == DESK_RIG_Y && rig->h == DESK_RIG_H);
    assert(haze->x == hits->x && haze->y == DESK_HAZE_Y && haze->h == DESK_HAZE_H);
    assert(rig->y == room->y + room->h + DESK_GAP);
    assert(haze->y == DESK_HITS_END + DESK_GAP && haze->y + haze->h == DESK_CONTENT_END);
    // HUMO AMBIENTE: OFF a full-height segment at the left, the four rhythms
    // two rows deep beside it, in map order.
    const struct desk_placement *off = find(&l, map.count + 2, 0);
    const char *rhythms[] = { "cada 1 min", "cada 2 min", "cada 4 min", "cada 8 min" };
    assert(off && off->tile == TILE_SEGMENT && inside(off, haze) && off->x == haze->x + DESK_PANEL_PAD);
    assert(off->y == haze->y + DESK_PANEL_TOP && off->y + off->h == haze->y + haze->h - DESK_PANEL_PAD);
    for (int k = 0; k < 4; k++) {
        const struct desk_placement *seg = find(&l, control_named(&map, rhythms[k]), 0);
        assert(seg && seg->tile == TILE_SEGMENT && inside(seg, haze) && seg->h >= 48 && seg->w >= 96);
        assert(seg->x >= off->x + off->w + DESK_GAP);
        assert(k % 2 == 0 ? seg->x == off->x + off->w + DESK_GAP
                          : seg->x + seg->w == haze->x + haze->w - DESK_PANEL_PAD);
        assert(k < 2 ? seg->y == off->y : seg->y + seg->h == off->y + off->h);
    }
    // COLOR DEL RIG: all seventeen colours, three rows of six, each at least the
    // 42x44 finger minimum, inside the panel.
    const struct desk_placement *mini = find(&l, control_named(&map, "Rig Rojo"), 0);
    assert(mini && mini->tile == TILE_MINI && inside(mini, rig) && mini->x == rig->x + DESK_PANEL_PAD);
    int swatches = 0, top_row = 0;
    for (int i = 0; i < l.placements; i++)
        if (l.placement[i].page == 0 && l.placement[i].tile == TILE_MINI) {
            const struct desk_placement *p = &l.placement[i];
            assert(inside(p, rig) && p->w >= 42 && p->h >= 44);
            assert(p->y == mini->y || p->y == mini->y + mini->h + DESK_GAP ||
                   p->y == mini->y + 2 * (mini->h + DESK_GAP));
            top_row += p->y == mini->y;
            swatches++;
        }
    assert(swatches == 17 && top_row == 6 && in_panel(&l, rig) == 18);
    const struct desk_placement *shot_mode = find(&l, map.count + 4, 0);
    assert(shot_mode && shot_mode->tile == TILE_COMPACT && inside(shot_mode, rig));
    assert(shot_mode->h >= 44 && shot_mode->y + shot_mode->h <= mini->y);
    const struct desk_placement *tempo = find(&l, map.count + 3, 0);
    assert(tempo && tempo->tile == TILE_TEMPO && tempo->x + tempo->w == DESK_CONTENT_X + DESK_CONTENT_W);
    assert(tempo->x == DESK_TEMPO_X && tempo->y == DESK_TEMPO_Y && tempo->h == DESK_TEMPO_H);
    assert(tempo->x == room->x + room->w + DESK_GAP && tempo->y + tempo->h + DESK_GAP == hits->y);
    // Every DIRECTO target is at least 48 px each way, but for the rig's
    // colours, ten in a third of the row, which keep the 42x44 finger minimum.
    for (int i = 0; i < l.placements; i++)
        if (l.placement[i].page == 0)
            assert(l.placement[i].w >= (l.placement[i].tile == TILE_MINI ? 42 : 48) &&
                   l.placement[i].h >= (l.placement[i].tile == TILE_COMPACT ? 44 : 48));
    assert(strcmp(l.title[0], "SHOW") == 0 && l.banks[0] == 1);
    // color-beam vanished from LIVE. The new colores-completos hook on
    // COLOR retains the colour-toggle role, as a cue beside the other hooks.
    int colours = -1;
    for (int i = 0; i < map.count; i++)
        if (strcmp(map.control[i].key, "colores-completos") == 0)
            colours = i;
    assert(colours >= 0);
    const struct desk_placement *hook = find(&l, colours, map.control[colours].page);
    // A family's hooks run four to a row across its content, 64 tall.
    int hook_w = (DESK_EFFECTS_W - 2 * DESK_PANEL_PAD - 3 * DESK_GAP) / 4;
    assert(hook && hook->tile == TILE_CUE && hook->w == hook_w && hook->h == 64);
    for (int page = 1; page < l.pages; page++) {
        if (l.tab[page] != DESK_TAB_EFECTOS)
            continue;
        for (int i = 0; i < l.placements; i++) {
            const struct desk_placement *p = &l.placement[i];
            if (p->page != page || p->control >= map.count || map.control[p->control].role != MAP_ROLE_HOOK)
                continue;
            int in_row = 0;
            for (int j = 0; j < l.placements; j++)
                if (l.placement[j].page == page && l.placement[j].bank == p->bank &&
                    l.placement[j].y == p->y && l.placement[j].control < map.count &&
                    map.control[l.placement[j].control].role == MAP_ROLE_HOOK)
                    in_row++;
            int sec_hooks = 0;
            for (int j = 0; j < map.count; j++)
                sec_hooks += map.control[j].page == page && map.control[j].role == MAP_ROLE_HOOK &&
                             map.control[j].section == map.control[p->control].section;
            assert(p->w == hook_w && (in_row == 4 || in_row == sec_hooks % 4 || in_row == sec_hooks));
        }
    }
    {
        // Turn one LIVE accent into a toggle: it stays among the pads, in
        // its map place, as a cue, first in the column.
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
        const struct desk_placement *cue = find(&show, accents->first, 0);
        const struct desk_placement *next = find(&show, accents->first + 1, 0);
        assert(cue && cue->tile == TILE_CUE && next && next->tile == TILE_HOLD);
        assert(cue->y == next->y && cue->x < next->x && cue->x == DESK_HITS_X && cue->y == DESK_HITS_TOP);
        // Without the tempo, the colour grid still stays below the modes.
        const struct desk_heading *wide = heading_named(&show, 0, "COLOR DEL RIG");
        assert(wide && wide->x == DESK_CONTENT_X && wide->w == DESK_HERO_W);
    }
    // COLOR keeps its picks 128 wide, five to a row inside their panel right
    // of the families, 96 tall for their square, and its colour hits as holds.
    const struct desk_placement *rojo = find(&l, control_named(&map, "Rig Rojo"), 1);
    assert(rojo && rojo->tile == TILE_SWATCH && rojo->w == 128 && rojo->h == 96);
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
    // Panels: a family's never overlap and stay in its content; DIRECTO's
    // are panels too, inside the content and clear of each other, but for the
    // holds' heading, a line over its pads.
    for (int i = 0; i < l.headings; i++) {
        const struct desk_heading *a = &l.heading[i];
        if (l.tab[a->page] != DESK_TAB_EFECTOS) {
            assert((a->h > 0 || a == hits) && a->x >= DESK_CONTENT_X &&
                   a->x + a->w <= DESK_CONTENT_X + DESK_CONTENT_W);
            assert(a->y >= DESK_CONTENT_Y && a->y + a->h <= DESK_CONTENT_END);
            for (int j = i + 1; j < l.headings; j++) {
                const struct desk_heading *b = &l.heading[j];
                if (b->page == a->page)
                    assert(a->x + a->w + DESK_GAP <= b->x || b->x + b->w + DESK_GAP <= a->x ||
                           a->y + a->h + DESK_GAP <= b->y || b->y + b->h + DESK_GAP <= a->y);
            }
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
    // GOBOS: "Gobo 1".."Gobo 17" are numbers, six to a row of 96+ px tiles
    // at least 48 tall; "Gobo Repartido 3" and "Gobo Shake - Gobo 9", which
    // end in a number under another stem, keep their mark tile on a row of
    // their own. PRISMA's bare numbers are numbers too; "Insert Prism" not.
    int gobos = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        const struct desk_placement *p = find(&l, i, c->page);
        if (c->role != MAP_ROLE_PICK || strcmp(map.page[c->page].key, "gobos") != 0)
            continue;
        int plain = strncmp(c->caption, "Gobo ", 5) == 0 && c->caption[5] >= '0' && c->caption[5] <= '9';
        assert(p->tile == (plain ? TILE_NUMBER : TILE_SWATCH));
        assert(p->w >= 96 && p->h >= 48);
        if (plain) {
            gobos++;
            int in_row = 0;
            for (int j = 0; j < l.placements; j++)
                if (l.placement[j].page == p->page && l.placement[j].bank == p->bank &&
                    l.placement[j].y == p->y && l.placement[j].tile != TILE_MASTER &&
                    l.placement[j].tile != TILE_PANIC) {
                    assert(l.placement[j].tile == TILE_NUMBER);
                    in_row++;
                }
            assert(in_row == 6 || in_row == 17 % 6);
        }
    }
    assert(gobos == 17 && l.banks[4] == 2);
    assert(find(&l, control_named(&map, "1 y 3"), 5)->tile == TILE_NUMBER);
    assert(find(&l, control_named(&map, "4"), 5)->tile == TILE_NUMBER);
    assert(find(&l, control_named(&map, "Insert Prism"), 5)->tile == TILE_SWATCH);
    assert(find(&l, control_named(&map, "Prisma Giro Rapido"), 5)->tile == TILE_SWATCH);
    assert(find(&l, control_named(&map, "Effect 42"), 2)->tile == TILE_NUMBER);
    assert(find(&l, control_named(&map, "Paneles Manual"), 2)->tile == TILE_SWATCH);
    // CABEZAS: every pick that leads with a figure's name draws it, the rest
    // keep their glyph; six to a row, over two banks.
    int figures = 0, glyphs = 0;
    for (int i = 0; i < map.count; i++) {
        const struct map_control *c = &map.control[i];
        if (c->role != MAP_ROLE_PICK || strcmp(map.page[c->page].key, "heads") != 0)
            continue;
        const struct desk_placement *p = find(&l, i, c->page);
        assert(p->w >= 96 && p->h >= 48 && p->w * 6 + 5 * DESK_GAP <= DESK_EFFECTS_W - 2 * DESK_PANEL_PAD);
        if (p->tile == TILE_TRAJECTORY)
            figures++;
        else if (p->tile == TILE_SWATCH && c->icon[0])
            glyphs++;
        else
            assert(0);
    }
    assert(figures == 21 && glyphs == 5 && l.banks[3] == 2);
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
