#include "qlc_console_set_host.h"

#include "qlc_session.h"

void qlc_console_set_host(void *ctx, const char *host, int port) {
    qlc_session_set_host(ctx, host, port);
}
