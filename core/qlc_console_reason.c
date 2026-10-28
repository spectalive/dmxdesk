#include "qlc_console_reason.h"

#include "qlc_session.h"

const char *qlc_console_reason(void *ctx) {
    return qlc_session_reason(ctx);
}
