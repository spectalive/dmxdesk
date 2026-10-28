#include "osc_link_close.h"

#include <unistd.h>

void osc_link_close(struct osc_link *l) {
    if (l->fd >= 0)
        close(l->fd);
    l->fd = -1;
    l->snapshot_pending = 0;
}
