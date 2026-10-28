#include "osc_console_recv.h"

#include "osc_event_of.h"
#include "osc_link.h"
#include "osc_link_next_msg.h"
#include "osc_map_apply.h"

int osc_console_recv(void *ctx, struct desk_event *out) {
    struct osc_link *l = ctx;
    struct osc_msg msg;
    if (!osc_link_next_msg(l, &msg))
        return 0;
    *out = osc_event_of(&l->map, osc_map_apply(&l->map, &msg));
    return 1;
}
