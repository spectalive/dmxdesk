#include "qlc_console_pollfds.h"

#include "qlc_session.h"

int qlc_console_pollfds(void *ctx, struct pollfd *fds, int cap) {
    return qlc_session_pollfds(ctx, fds, cap);
}
