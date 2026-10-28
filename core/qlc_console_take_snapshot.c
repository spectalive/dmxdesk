#include "qlc_console_take_snapshot.h"

#include "qlc_session.h"

int qlc_console_take_snapshot(void *ctx, struct vc_doc *out) {
    return qlc_session_take_snapshot(ctx, out);
}
