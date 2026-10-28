#include "qlc_console_last_rtt.h"

#include "qlc_session.h"

int qlc_console_last_rtt(void *ctx) {
    return qlc_session_last_rtt(ctx);
}
