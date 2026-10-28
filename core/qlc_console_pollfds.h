// The QLC+ session's sockets.
#ifndef QLC_CONSOLE_POLLFDS_H
#define QLC_CONSOLE_POLLFDS_H

#include <poll.h>

int qlc_console_pollfds(void *ctx, struct pollfd *fds, int cap);

#endif
