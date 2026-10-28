#include "osc_loop_pop.h"

int osc_loop_pop(struct osc_link *l, struct osc_msg *out) {
    if (!l->loop_count)
        return 0;
    *out = l->loop[l->loop_head];
    l->loop_head = (l->loop_head + 1) % OSC_LOOP_MAX;
    l->loop_count--;
    return 1;
}
