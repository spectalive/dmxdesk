#include "osc_console_step.h"

#include <stdio.h>

#include "osc_link.h"
#include "osc_link_open.h"

enum desk_console_link osc_console_step(void *ctx, int64_t now) {
    struct osc_link *l = ctx;
    if (l->fd < 0 && now >= l->retry_at) {
        if (osc_link_open(l) == 0) {
            l->snapshot_pending = 1;
            fprintf(stderr, "osc: sending to %s:%d\n", l->host, l->port);
        } else {
            l->retry_at = now + OSC_RETRY_MS;
        }
    }
    return l->fd >= 0 ? DESK_CONSOLE_READY : DESK_CONSOLE_DOWN;
}
