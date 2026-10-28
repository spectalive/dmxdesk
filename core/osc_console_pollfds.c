#include "osc_console_pollfds.h"

#include "osc_link.h"

int osc_console_pollfds(void *ctx, struct pollfd *fds, int cap) {
    struct osc_link *l = ctx;
    if (l->fd < 0 || cap < 1)
        return 0;
    fds[0] = (struct pollfd){ .fd = l->fd, .events = POLLIN };
    return 1;
}
