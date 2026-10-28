#include "osc_link_next_msg.h"

#include <string.h>
#include <sys/socket.h>

#include "osc_decode.h"
#include "osc_loop_pop.h"
#include "osc_same_host.h"

int osc_link_next_msg(struct osc_link *l, struct osc_msg *out) {
    if (osc_loop_pop(l, out))
        return 1;
    if (l->fd < 0)
        return 0;
    char buf[1024];
    struct sockaddr_storage from;
    socklen_t from_len = sizeof from;
    memset(&from, 0, sizeof from);
    ssize_t n = recvfrom(l->fd, buf, sizeof buf, 0, (struct sockaddr *)&from, &from_len);
    if (n < 0)
        return 0;
    if (!l->map.feedback || !osc_same_host(&from, &l->dest))
        memset(out, 0, sizeof *out);
    else
        osc_decode(buf, (size_t)n, out);
    return 1;
}
