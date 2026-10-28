#include "osc_link_next_msg.h"

#include <sys/socket.h>

#include "osc_decode.h"
#include "osc_loop_pop.h"

int osc_link_next_msg(struct osc_link *l, struct osc_msg *out) {
    if (osc_loop_pop(l, out))
        return 1;
    if (l->fd < 0)
        return 0;
    char buf[1024];
    ssize_t n = recv(l->fd, buf, sizeof buf, 0);
    if (n < 0)
        return 0;
    osc_decode(buf, (size_t)n, out);
    return 1;
}
