// One poll over the touch source, the power key, the supplicant's two
// sockets and the master's; remembers which devices became readable.
#ifndef LINUX_DESK_WAIT_H
#define LINUX_DESK_WAIT_H

#include <poll.h>

void linux_desk_wait(void *ctx, const struct pollfd *master, int count, int timeout_ms);

#endif
