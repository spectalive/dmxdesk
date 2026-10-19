#include "desk_damage_running.h"

#include "desk_layout.h"
#include "desk_panel_holds.h"
#include "desk_tab.h"

void desk_damage_running(struct desk_model *m, int control) {
    const struct desk_control *c = &m->control[control];
    int state = c->kind == DESK_CUE && c->role == MAP_ROLE_STATE;
    if (state) {
        int word_x = DESK_TAB_X + DESK_TABS * (DESK_TAB_W + DESK_TAB_GAP);
        desk_damage_rect(m, word_x, 0, DESK_STATUS_END - word_x, DESK_BAR_H);
    }
    for (int i = 0; i < m->layout.headings; i++) {
        const struct desk_heading *h = &m->layout.heading[i];
        if (h->page != m->page || h->bank != m->bank || h->h <= 0)
            continue;
        int speaks = h->kind == DESK_PANEL_ROOM && state;
        for (int k = 0; k < m->layout.placements && !speaks; k++)
            speaks = h->kind == DESK_PANEL_NAMES_RUNNING && m->layout.placement[k].control == control &&
                     desk_panel_holds(h, &m->layout.placement[k]);
        if (speaks)
            desk_damage_rect(m, h->x, h->y, h->w, h->h);
    }
}
