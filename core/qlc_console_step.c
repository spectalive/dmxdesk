#include "qlc_console_step.h"

#include "qlc_console_link_of.h"
#include "qlc_session.h"

enum desk_console_link qlc_console_step(void *ctx, int64_t now) {
    return qlc_console_link_of(qlc_session_step(ctx, now));
}
