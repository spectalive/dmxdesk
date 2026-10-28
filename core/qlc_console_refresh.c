#include "qlc_console_refresh.h"

#include "qlc_session.h"

void qlc_console_refresh(void *ctx, int64_t now) {
    qlc_session_refresh(ctx, now);
}
