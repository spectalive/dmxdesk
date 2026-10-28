// desk_console.pollfds for OSC: the socket, while it is open.
#ifndef OSC_CONSOLE_POLLFDS_H
#define OSC_CONSOLE_POLLFDS_H

#include <poll.h>

int osc_console_pollfds(void *ctx, struct pollfd *fds, int cap);

#endif
