#include "desk_panel_running.h"

#include "desk_panel_holds.h"

const struct desk_control *desk_panel_running(const struct desk_model *m, const struct desk_heading *h) {
    for (int i = 0; i < m->layout.placements; i++) {
        const struct desk_placement *p = &m->layout.placement[i];
        if (!desk_panel_holds(h, p) || p->control < 0 || p->control >= m->count)
            continue;
        const struct desk_control *c = &m->control[p->control];
        if (c->enabled && c->state == DESK_ON)
            return c;
    }
    return NULL;
}
