#include "desk_panel_title.h"

#include "desk_panel_holds.h"

const char *desk_panel_title(const struct desk_model *m, const struct desk_heading *h) {
    for (int i = 0; i < m->layout.placements; i++) {
        const struct desk_placement *p = &m->layout.placement[i];
        if (!desk_panel_holds(h, p) || p->control < 0 || p->control >= m->count)
            continue;
        const struct desk_control *c = &m->control[p->control];
        if (c->kind == DESK_COLOR_MODE && c->enabled && c->state == DESK_ON)
            return c->label;
    }
    return h->text;
}
