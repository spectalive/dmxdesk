#include "osc_console_refresh.h"

#include "osc_link.h"

void osc_console_refresh(void *ctx, int64_t now) {
    (void)now;
    struct osc_link *l = ctx;
    if (l->fd >= 0)
        l->snapshot_pending = 1;
}
