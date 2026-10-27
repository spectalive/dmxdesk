#include "desk_inert_mask.h"

void desk_inert_mask(const struct desk_model *m, const struct desk_setup *s, const struct desk_speed *speed, unsigned char *mask) {
    // Layout contract, independent of observed successful hits: spare glass,
    // headings, status and gutters are intentionally inert. Reserve every
    // enabled control envelope before granting that background exemption.
    // The audit rejects any exemption covering an action or enabled drawing.
    desk_region_mark(mask, (struct desk_rect){0, 0, DESK_W, DESK_H}, 1);
    if (!s->open) {
        for (int i = 0; i < DESK_TABS; i++)
            if (desk_tab_page(m, i) >= 0)
                desk_region_mark(mask, desk_view_tab(i), 0);
        if (m->layout.tab[m->page] == DESK_TAB_EFECTOS)
            for (int i = 0; i < m->layout.families; i++)
                desk_region_mark(mask, desk_view_family(i, m->layout.families), 0);
        if (m->layout.banks[m->page] > 1)
            for (int i = 0; i < m->layout.banks[m->page]; i++) {
                struct desk_rect pager = desk_view_pager(i, &m->layout, m->page);
                // The pager owns its full column to the glass edge. Do not
                // derive this obligation from desk_pager_hit: a shortened
                // input rectangle must leave a detectable dead strip here.
                pager.h = DESK_H - pager.y;
                desk_region_mark(mask, pager, 0);
            }
        if (m->page == m->layout.speed_page)
            desk_speed_inert_mask(speed, mask);
    } else {
        desk_setup_inert_mask(s, mask);
    }
    for (int i = 0; i < m->layout.placements; i++) {
        const struct desk_placement *p = &m->layout.placement[i];
        if (p->page != m->page || p->bank != m->bank || p->control < 0 || p->control >= m->count ||
            (s->open && p->x < SETUP_SHEET_W) || !m->control[p->control].enabled)
            continue;
        struct desk_rect r = {p->x, p->y, p->w, p->h};
        if (m->control[p->control].kind == DESK_TEMPO) {
            desk_speed_tempo_tap_rect(p->x, p->y, p->w, p->h, &r.x, &r.y, &r.w, &r.h);
            desk_region_mark(mask, r, 0);
            for (int step = SPEED_T_BPM_DOWN; step <= SPEED_T_BPM_UP; step++) {
                desk_speed_tempo_step_rect(p->x, p->y, p->w, p->h,
                                           (enum speed_target)step, &r.x, &r.y, &r.w, &r.h);
                if (desk_speed_target_enabled(speed, 0, (enum speed_target)step))
                    desk_region_mark(mask, r, 0);
            }
        } else
            desk_region_mark(mask, r, 0);
    }
    desk_region_mark(mask, desk_view_gear(), 0);
    desk_region_mark(mask, desk_view_lock_target(), 0);
}
