#include "osc_loop_push.h"

void osc_loop_push(struct osc_link *l, const struct osc_msg *msg) {
    if (l->loop_count == OSC_LOOP_MAX) {
        l->loop_head = (l->loop_head + 1) % OSC_LOOP_MAX;
        l->loop_count--;
    }
    l->loop[(l->loop_head + l->loop_count) % OSC_LOOP_MAX] = *msg;
    l->loop_count++;
}
