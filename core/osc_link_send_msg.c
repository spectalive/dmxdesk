#include "osc_link_send_msg.h"

#include <sys/socket.h>

#include "osc_encode.h"

int osc_link_send_msg(struct osc_link *l, const struct osc_msg *msg) {
    char buf[OSC_ADDRESS_MAX + 16];
    int n = osc_encode(msg, buf, sizeof buf);
    if (n < 0 || l->fd < 0)
        return -1;
    ssize_t sent = sendto(l->fd, buf, (size_t)n, 0, (const struct sockaddr *)&l->dest,
                          l->dest_len);
    return sent == n ? 0 : -1;
}
