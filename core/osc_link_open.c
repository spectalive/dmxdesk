#include "osc_link_open.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "osc_any_address.h"
#include "osc_resolve.h"

int osc_link_open(struct osc_link *l) {
    if (osc_resolve(l->host, l->port, &l->dest, &l->dest_len, l->reason, sizeof l->reason) != 0)
        return -1;
    int fd = socket(l->dest.ss_family, SOCK_DGRAM, 0);
    if (fd < 0 || fcntl(fd, F_SETFL, O_NONBLOCK) != 0 || fcntl(fd, F_SETFD, FD_CLOEXEC) != 0) {
        snprintf(l->reason, sizeof l->reason, "socket: %s", strerror(errno));
        if (fd >= 0)
            close(fd);
        return -1;
    }
    struct sockaddr_storage local;
    socklen_t local_len = osc_any_address(l->dest.ss_family, l->listen_port, &local);
    if (bind(fd, (struct sockaddr *)&local, local_len) != 0) {
        snprintf(l->reason, sizeof l->reason, "listen port %d: %s", l->listen_port,
                 strerror(errno));
        close(fd);
        return -1;
    }
    l->fd = fd;
    return 0;
}
