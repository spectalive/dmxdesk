#include "osc_console_take_snapshot.h"

#include <stdio.h>

#include "desk_surface_copy.h"
#include "osc_link.h"

int osc_console_take_snapshot(void *ctx, struct desk_surface *out) {
    struct osc_link *l = ctx;
    if (!l->snapshot_pending)
        return 0;
    l->snapshot_pending = 0;
    if (desk_surface_copy(&l->map.surface, out) != 0) {
        fprintf(stderr, "osc: no memory for the console's surface\n");
        return 0;
    }
    return 1;
}
